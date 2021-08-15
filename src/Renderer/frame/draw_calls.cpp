/*****************************************************
 * Partial Class: Renderer
 * Members: Frame - Draw Calls (Private)
 *****************************************************/

#include "Renderer.hpp"

using namespace sdfRay4d;

//void Renderer::buildDrawCallsForItems()
//{
//  VkCommandBuffer cb = m_vkWindow->currentCommandBuffer();
//
//  m_deviceFuncs->vkCmdBindPipeline(cb, VK_PIPELINE_BIND_POINT_GRAPHICS, _material->pipeline);
//
//  VkDeviceSize vbOffset = 0;
//  m_deviceFuncs->vkCmdBindVertexBuffers(cb, 0, 1, &m_sdfUniformBuffer, &vbOffset);
////  m_deviceFuncs->vkCmdBindVertexBuffers(cb, 1, 1, &m_instBuf, &vbOffset);
//
//  // Now provide offsets so that the two dynamic buffers point to the
//  // beginning of the vertex and fragment uniform data for the current frame.
//  uint32_t frameUniOffset = m_vkWindow->currentFrame() * (_material->vertUniSize + _material->fragUniSize);
//  uint32_t frameUniOffsets[] = { frameUniOffset, frameUniOffset };
//  m_deviceFuncs->vkCmdBindDescriptorSets(cb, VK_PIPELINE_BIND_POINT_GRAPHICS, _material->pipelineLayout, 0, 1,
//                                         &_material->descSet, 2, frameUniOffsets);
//
//  if (m_animating)
//    m_rotation += 0.5;
//
//  if (m_animating || m_vpDirty) {
//    if (m_vpDirty)
//      --m_vpDirty;
//    QMatrix4x4 vp, model;
//    QMatrix3x3 modelNormal;
//    QVector3D eyePos;
//    getMatrices(&vp, &model, &modelNormal, &eyePos);
//
//    // Map the uniform data for the current frame, ignore the geometry data at
//    // the beginning and the uniforms for other frames.
//    quint8 *p;
//    VkResult err = m_deviceFuncs->vkMapMemory(dev, m_bufMem,
//                                              _material->uniMemStartOffset + frameUniOffset,
//                                              _material->vertUniSize + _material->fragUniSize,
//                                              0, reinterpret_cast<void **>(&p));
//    if (err != VK_SUCCESS)
//      qFatal("Failed to map memory: %d", err);
//
//    // Vertex shader uniforms
//    memcpy(p, vp.constData(), 64);
//    memcpy(p + 64, model.constData(), 64);
//    const float *mnp = modelNormal.constData();
//    memcpy(p + 128, mnp, 12);
//    memcpy(p + 128 + 16, mnp + 3, 12);
//    memcpy(p + 128 + 32, mnp + 6, 12);
//
//    // Fragment shader uniforms
//    p += _material->vertUniSize;
//    writeFragUni(p, eyePos);
//
//    m_deviceFuncs->vkUnmapMemory(dev, m_bufMem);
//  }
//
//  m_deviceFuncs->vkCmdDraw(cb, (m_useLogo ? m_logoMesh.data() : m_blockMesh.data())->vertexCount, m_instCount, 0, 0);
//}

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

  auto mvp = m_proj * m_camera.viewMatrix();// * m_floorModel;

  float fragmentConstants[5] = {
    _extentWidth,
    _extentHeight,

    (float)1,
    (float)1,

    (float)1
//      SDL_GetTicks() / 1000.0f
  };

//  qDebug("sizeof(QMatrix4x4): %u ", sizeof(mvp));

//  m_deviceFuncs->vkCmdPushConstants(cmdBuffer, _material->pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(mvp), mvp.constData());

//  float color[] = { 0.3f, 0.3f, 0.3f };

//  qDebug("sizeof(mvp): %u ", sizeof(mvp));

  m_deviceFuncs->vkCmdPushConstants(
    _cmdBuffer,
    _material->pipelineLayout,
    VK_SHADER_STAGE_FRAGMENT_BIT,
    0/*sizeof(mvp) - 4*/,
    sizeof(fragmentConstants), fragmentConstants
//    sizeof(mvp), mvp.constData()
  );

  uint32_t frameUniOffset = m_vkWindow->currentFrame() * (
    m_sdfrMaterial->vertUniSize +
    m_sdfrMaterial->fragUniSize
  );
  uint32_t frameUniOffsets[] = { frameUniOffset, frameUniOffset };
  m_deviceFuncs->vkCmdBindDescriptorSets(
    _cmdBuffer,
    VK_PIPELINE_BIND_POINT_GRAPHICS,
    m_sdfrMaterial->pipelineLayout,
    0, 1,
    &m_sdfrMaterial->descSet,
    0,
    frameUniOffsets
  );

//  uint32_t frameBufOffset = m_vkWindow->currentFrame() * (
//    m_sdfrMaterial->vertUniSize +
//    m_sdfrMaterial->fragUniSize
//  );
//  uint32_t frameBufOffsets[] = { frameBufOffset, frameBufOffset };
//  m_deviceFuncs->vkCmdBindDescriptorSets(
//    _cmdBuffer,
//    VK_PIPELINE_BIND_POINT_GRAPHICS,
//    m_sdfrMaterial->pipelineLayout,
//    1, 1,
//    &m_sdfrMaterial->descSets[1],
//    1,
//    frameBufOffsets
//  );

  m_deviceFuncs->vkCmdDraw(
    _cmdBuffer,
    2 * 3,1,
    0,0
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
//  m_deviceFuncs->vkCmdBindVertexBuffers(
//    _cmdBuffer,
//    1, 1, &m_instBuf, &vbOffset);

  // Now provide offsets so that the two dynamic buffers point to the
  // beginning of the vertex and fragment uniform data for the current frame.
  uint32_t frameUniOffset = m_vkWindow->currentFrame() * (
    m_actorMaterial->vertUniSize +
    m_actorMaterial->fragUniSize
  );
  uint32_t frameUniOffsets[] = { frameUniOffset, frameUniOffset };
  m_deviceFuncs->vkCmdBindDescriptorSets(
    _cmdBuffer,
    VK_PIPELINE_BIND_POINT_GRAPHICS,
    m_actorMaterial->pipelineLayout,
    0, 1,
    &m_actorMaterial->descSet,
    2,
    frameUniOffsets
  );

  if (m_vpDirty) {
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




void Renderer::getMatrices(
  QMatrix4x4 *vp,
  QMatrix4x4 *model,
  QMatrix3x3 *modelNormal,
  QVector3D *eyePos
)
{
  model->setToIdentity();
  model->rotate(m_rotation, 1, 1, 0);

  *modelNormal = model->normalMatrix();

  QMatrix4x4 view = m_camera.viewMatrix();
  *vp = m_proj * view;

  *eyePos = view.inverted().column(3).toVector3D();
}

void Renderer::writeFragUni(
  quint8 *p,
  const QVector3D &eyePos
)
{
  float ECCameraPosition[] = { eyePos.x(), eyePos.y(), eyePos.z() };
  memcpy(p, ECCameraPosition, 12);
  p += 16;

  // Material
  float ka[] = { 0.05f, 0.05f, 0.05f };
  memcpy(p, ka, 12);
  p += 16;

  float kd[] = { 0.7f, 0.7f, 0.7f };
  memcpy(p, kd, 12);
  p += 16;

  float ks[] = { 0.66f, 0.66f, 0.66f };
  memcpy(p, ks, 12);
  p += 16;

  QVector3D m_lightPos(0.0f, 0.0f, 25.0f);

  // Light parameters
  float ECLightPosition[] = { m_lightPos.x(), m_lightPos.y(), m_lightPos.z() };
  memcpy(p, ECLightPosition, 12);
  p += 16;

  float att[] = { 1, 0, 0 };
  memcpy(p, att, 12);
  p += 16;

  float color[] = { 1.0f, 1.0f, 1.0f };
  memcpy(p, color, 12);
  p += 12; // next we have two floats which have an alignment of 4, hence 12 only

  float intensity = 0.8f;
  memcpy(p, &intensity, 4);
  p += 4;

  float specularExp = 150.0f;
  memcpy(p, &specularExp, 4);
  p += 4;

//  auto nearPlane = m_nearPlane;
  memcpy(p, &m_nearPlane, 4);
  p += 4;

//  auto farPlane = m_farPlane;
  memcpy(p, &m_farPlane, 4);
  p += 4;
}
