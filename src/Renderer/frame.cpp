/*****************************************************
 * Partial Class: Renderer
 * Members: Frame Method Override (Public)
 *****************************************************/

#include "Renderer.hpp"

using namespace sdfRay4d;

void Renderer::startNextFrame()
{
  // For demonstration purposes offload the command buffer generation onto a
  // worker thread and continue with the frame submission only when it has
  // finished.
  Q_ASSERT(!m_isFramePending);
  m_isFramePending = true;

  auto future = QtConcurrent::run(this, &Renderer::buildFrame);
  m_frameWatcher.setFuture(future);
}

void Renderer::buildFrame()
{
  QMutexLocker locker(&m_guiMutex);

//  const int concurrentFrameCount = m_vkWindow->concurrentFrameCount();
//
//  createUploadBuffer(concurrentFrameCount);

  ensureBuffers();
//  ensureInstanceBuffer();
  m_pipelinesFuture.waitForFinished();

  const QSize sz = m_vkWindow->swapChainImageSize();

  VkClearColorValue clearColor = {{ 0.67f, 0.84f, 0.9f, 1.0f }};
  VkClearDepthStencilValue clearDS = { 1, 0 };
  VkClearValue clearValues[3];
  memset(clearValues, 0, sizeof(clearValues));
  clearValues[0].color = clearValues[2].color = clearColor;
  clearValues[1].depthStencil = clearDS;

  VkRenderPassBeginInfo rpBeginInfo;
  memset(&rpBeginInfo, 0, sizeof(rpBeginInfo));
  rpBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
  rpBeginInfo.renderPass = m_vkWindow->defaultRenderPass();
  rpBeginInfo.framebuffer = m_vkWindow->currentFramebuffer();
  rpBeginInfo.renderArea.extent.width = sz.width();
  rpBeginInfo.renderArea.extent.height = sz.height();
  rpBeginInfo.clearValueCount = m_vkWindow->sampleCountFlagBits() > VK_SAMPLE_COUNT_1_BIT ? 3 : 2;
  rpBeginInfo.pClearValues = clearValues;

  VkCommandBuffer cmdBuf = m_vkWindow->currentCommandBuffer();

  m_deviceFuncs->vkCmdBeginRenderPass(cmdBuf, &rpBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

    VkViewport viewport = {
      0.0f, 0.0f,
      float(sz.width()), float(sz.height()),
      0, 1
    };
    m_deviceFuncs->vkCmdSetViewport(cmdBuf, 0, 1, &viewport);

    VkRect2D scissor = {
      { 0, 0 },
      { uint32_t(sz.width()), uint32_t(sz.height()) }
    };
    m_deviceFuncs->vkCmdSetScissor(cmdBuf, 0, 1, &scissor);

    buildDrawCallsForFloor();
  //  buildDrawCallsForItems();

  m_deviceFuncs->vkCmdEndRenderPass(cmdBuf);
}
