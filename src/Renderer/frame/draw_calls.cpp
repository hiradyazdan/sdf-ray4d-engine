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
//  m_deviceFuncs->vkCmdBindPipeline(cb, VK_PIPELINE_BIND_POINT_GRAPHICS, m_objMaterial.pipeline);
//
//  VkDeviceSize vbOffset = 0;
//  m_deviceFuncs->vkCmdBindVertexBuffers(cb, 0, 1, &m_uniformBuffer, &vbOffset);
////  m_deviceFuncs->vkCmdBindVertexBuffers(cb, 1, 1, &m_instBuf, &vbOffset);
//
//  // Now provide offsets so that the two dynamic buffers point to the
//  // beginning of the vertex and fragment uniform data for the current frame.
//  uint32_t frameUniOffset = m_vkWindow->currentFrame() * (m_objMaterial.vertUniSize + m_objMaterial.fragUniSize);
//  uint32_t frameUniOffsets[] = { frameUniOffset, frameUniOffset };
//  m_deviceFuncs->vkCmdBindDescriptorSets(cb, VK_PIPELINE_BIND_POINT_GRAPHICS, m_objMaterial.pipelineLayout, 0, 1,
//                                         &m_objMaterial.descSet, 2, frameUniOffsets);
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
//                                              m_objMaterial.uniMemStartOffset + frameUniOffset,
//                                              m_objMaterial.vertUniSize + m_objMaterial.fragUniSize,
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
//    p += m_objMaterial.vertUniSize;
//    writeFragUni(p, eyePos);
//
//    m_deviceFuncs->vkUnmapMemory(dev, m_bufMem);
//  }
//
//  m_deviceFuncs->vkCmdDraw(cb, (m_useLogo ? m_logoMesh.data() : m_blockMesh.data())->vertexCount, m_instCount, 0, 0);
//}

void Renderer::buildDrawCalls(CmdBuffer &_cmdBuffer, QSize &_swapChainImgSize)
{
  m_deviceFuncs->vkCmdBindPipeline(
    _cmdBuffer,
    VK_PIPELINE_BIND_POINT_GRAPHICS,
    m_objMaterial.pipeline
  );

  device::Size vbOffset = 0;
  m_deviceFuncs->vkCmdBindVertexBuffers(
    _cmdBuffer,
    0,1,
    &m_uniformBuffer, &vbOffset
  );

//  QMatrix4x4 mvp = m_proj; //* m_cam.viewMatrix() * m_floorModel;

  float fragmentConstants[4] = {
    (float)_swapChainImgSize.width(),
    (float)_swapChainImgSize.height(),
//    (float)1,
//    (float)1,
//      SDL_GetTicks() / 1000.0f
  };

//  qDebug("sizeof(QMatrix4x4): %u ", sizeof(mvp));

//  m_deviceFuncs->vkCmdPushConstants(cmdBuffer, m_objMaterial.pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(mvp), mvp.constData());

//  float color[] = { 0.3f, 0.3f, 0.3f };

//  qDebug("sizeof(mvp): %u ", sizeof(mvp));

  m_deviceFuncs->vkCmdPushConstants(
    _cmdBuffer,
    m_objMaterial.pipelineLayout,
    VK_SHADER_STAGE_FRAGMENT_BIT,
    0/*sizeof(mvp) - 4*/,
    sizeof(fragmentConstants), fragmentConstants//mvp.constData()
  );

  m_deviceFuncs->vkCmdDraw(
    _cmdBuffer,
    3,1,
    0,0
  );
}
