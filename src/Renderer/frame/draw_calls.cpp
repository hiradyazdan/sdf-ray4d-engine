/*****************************************************
 * Partial Class: Renderer
 * Members: Frame - Draw Calls (Private)
 *****************************************************/

#include "Renderer.hpp"

using namespace sdfRay4d;

void Renderer::createDepthDrawCalls(
  const MaterialPtr &_material,
  CmdBuffer &_cmdBuffer
)
{
  auto &material = m_depthMaterial;

  m_deviceFuncs->vkCmdBindPipeline(
    _cmdBuffer,
    VK_PIPELINE_BIND_POINT_GRAPHICS,
    material->pipeline
  );

  device::Size vbOffset = 0;
  m_deviceFuncs->vkCmdBindVertexBuffers(
    _cmdBuffer,
    0, 1,
    &m_depthVertexBuffer,
    &vbOffset
  );

  // the dynamic buffer points to the beginning of the vertex uniform data for the current frame.
  uint32_t frameUniOffset = m_vkWindow->currentFrame() * material->vertUniSize;
  uint32_t frameUniOffsets[] = { frameUniOffset };
  m_deviceFuncs->vkCmdBindDescriptorSets(
    _cmdBuffer,
    VK_PIPELINE_BIND_POINT_GRAPHICS,
    material->pipelineLayout,
    0, material->descSets.size(),
    material->descSets.data(),
    1,
    frameUniOffsets
  );

  /**
   * Map memory only if there's a change in view projection
   *
   * TODO:
   * better if map memory at the beginning and leave it mapped through the end
   * no more need to unmap memory
   */
  if (m_vpDirty)
  {
    if (m_vpDirty) --m_vpDirty;

    QMatrix4x4 vp, model;
    QMatrix3x3 modelNormal;
    QVector3D eyePos;
    getMatrices(&vp, &model, &modelNormal, &eyePos);

    // Map the uniform data for the current frame, ignore the geometry data at
    // the beginning and the uniforms for other frames.
    quint8 *p;
    auto result = m_deviceFuncs->vkMapMemory(
      m_device, m_bufferMemory,
      material->uniMemStartOffset + frameUniOffset,
      material->vertUniSize,
      0, reinterpret_cast<void **>(&p)
    );

    if (result != VK_SUCCESS)
    {
      qFatal("Failed to map memory: %d", result);
    }

    // Vertex shader uniforms
    memcpy(p, vp.constData(), 64);
    memcpy(p + 64, model.constData(), 64);
    const float *mnp = modelNormal.constData();
    memcpy(p + 128, mnp, 12);
    memcpy(p + 128 + 16, mnp + 3, 12);
    memcpy(p + 128 + 32, mnp + 6, 12);

    m_deviceFuncs->vkUnmapMemory(m_device, m_bufferMemory);
  }

  m_deviceFuncs->vkCmdDraw(
    _cmdBuffer,
    m_actorMesh.data()->vertexCount, // TODO: FIX
    1,
    0, 0
  );
}

void Renderer::createActorDrawCalls(
  const MaterialPtr &_material,
  CmdBuffer &_cmdBuffer
)
{
  m_deviceFuncs->vkCmdBindPipeline(
    _cmdBuffer,
    VK_PIPELINE_BIND_POINT_GRAPHICS,
    m_actorMaterial->pipeline
  );

  device::Size vbOffset = 0;
  m_deviceFuncs->vkCmdBindVertexBuffers(
    _cmdBuffer,
    0, 1,
    &m_actorVertexBuffer,
    &vbOffset
  );

  // the dynamic buffer points to the beginning of the vertex and fragment
  // uniform data for the current frame.
  uint32_t frameUniOffset = m_vkWindow->currentFrame() * (
    m_actorMaterial->vertUniSize +
    m_actorMaterial->fragUniSize
  );
  uint32_t frameUniOffsets[] = { frameUniOffset, frameUniOffset };
  m_deviceFuncs->vkCmdBindDescriptorSets(
    _cmdBuffer,
    VK_PIPELINE_BIND_POINT_GRAPHICS,
    m_actorMaterial->pipelineLayout,
    0, m_actorMaterial->descSets.size(),
    &m_actorMaterial->descSets[0], // Dynamic UBOs
    2,
    frameUniOffsets
  );

  if (m_vpDirty)
  {
    if (m_vpDirty) --m_vpDirty;

    QMatrix4x4 vp, model;
    QMatrix3x3 modelNormal;
    QVector3D eyePos;
    getMatrices(&vp, &model, &modelNormal, &eyePos);

    // Map the uniform data for the current frame, ignore the geometry data at
    // the beginning and the uniforms for other frames.
    quint8 *p;
    auto result = m_deviceFuncs->vkMapMemory(
      m_device, m_bufferMemory,
      m_actorMaterial->uniMemStartOffset + frameUniOffset,
      m_actorMaterial->vertUniSize + m_actorMaterial->fragUniSize,
      0, reinterpret_cast<void **>(&p)
    );
    if (result != VK_SUCCESS)
    {
      qFatal("Failed to map memory: %d", result);
    }

    // Vertex shader uniforms
    memcpy(p, vp.constData(), 64);
    memcpy(p + 64, model.constData(), 64);
    const float *mnp = modelNormal.constData();
    memcpy(p + 128, mnp, 12);
    memcpy(p + 128 + 16, mnp + 3, 12);
    memcpy(p + 128 + 32, mnp + 6, 12);

    // Fragment shader uniforms
    p += m_actorMaterial->vertUniSize;
    writeFragUni(p, eyePos);

    m_deviceFuncs->vkUnmapMemory(m_device, m_bufferMemory);
  }

  m_deviceFuncs->vkCmdDraw(
    _cmdBuffer,
    m_actorMesh.data()->vertexCount,
    1,
    0, 0
  );
}

void Renderer::createSDFRDrawCalls(
  const MaterialPtr &_material,
  CmdBuffer &_cmdBuffer,
  float _extentWidth,
  float _extentHeight
)
{
  m_deviceFuncs->vkCmdBindPipeline(
    _cmdBuffer,
    VK_PIPELINE_BIND_POINT_GRAPHICS, // TODO: VK_PIPELINE_BIND_POINT_COMPUTE
    _material->pipeline
  );

  device::Size vbOffset = 0;
  m_deviceFuncs->vkCmdBindVertexBuffers(
    _cmdBuffer,
    0,1,
    &m_sdfUniformBuffer, &vbOffset
  );

  float fragmentConstants[7] = {
    _extentWidth,
    _extentHeight,

    (float)1,
    (float)1,

    (float)1,

    m_nearPlane,
    m_farPlane
  };

  m_deviceFuncs->vkCmdPushConstants(
    _cmdBuffer,
    _material->pipelineLayout,
    VK_SHADER_STAGE_FRAGMENT_BIT,
    0/*sizeof(mvp) - 4*/,
    sizeof(fragmentConstants), fragmentConstants
  );

  m_deviceFuncs->vkCmdBindDescriptorSets(
    _cmdBuffer,
    VK_PIPELINE_BIND_POINT_GRAPHICS,
    m_sdfrMaterial->pipelineLayout,
    0, m_sdfrMaterial->descSets.size(),
    &m_sdfrMaterial->descSets[0], // Texture Sampler
    0,
    nullptr
  );

  m_deviceFuncs->vkCmdDraw(
    _cmdBuffer,
    2 * 3,1,
    0,0
  );
}


// TODO: modify these two below methods

void Renderer::getMatrices(
  QMatrix4x4 *_vp,
  QMatrix4x4 *_model,
  QMatrix3x3 *_modelNormal,
  QVector3D *_eyePos
)
{
  _model->setToIdentity();
  _model->rotate(m_rotation, 1, 1, 0);

  *_modelNormal = _model->normalMatrix();

  QMatrix4x4 view = m_camera.viewMatrix();
  *_vp = m_proj * view;

  *_eyePos = view.inverted().column(3).toVector3D();
}

void Renderer::writeFragUni(
  quint8 *_p,
  const QVector3D &_eyePos
)
{
  float ECCameraPosition[] = { _eyePos.x(), _eyePos.y(), _eyePos.z() };
  memcpy(_p, ECCameraPosition, 12);
  _p += 16;

  // Material
  float ka[] = { 0.05f, 0.05f, 0.05f };
  memcpy(_p, ka, 12);
  _p += 16;

  float kd[] = { 0.7f, 0.7f, 0.7f };
  memcpy(_p, kd, 12);
  _p += 16;

  float ks[] = { 0.66f, 0.66f, 0.66f };
  memcpy(_p, ks, 12);
  _p += 16;

  QVector3D lightPos(0.0f, 0.0f, 25.0f);

  // Light parameters
  float ECLightPosition[] = { lightPos.x(), lightPos.y(), lightPos.z() };
  memcpy(_p, ECLightPosition, 12);
  _p += 16;

  float att[] = { 1, 0, 0 };
  memcpy(_p, att, 12);
  _p += 16;

  float color[] = { 1.0f, 1.0f, 1.0f };
  memcpy(_p, color, 12);
  _p += 12; // next we have two floats which have an alignment of 4, hence 12 only

  float intensity = 0.8f;
  memcpy(_p, &intensity, 4);
  _p += 4;

  float specularExp = 150.0f;
  memcpy(_p, &specularExp, 4);
  _p += 4;
}
