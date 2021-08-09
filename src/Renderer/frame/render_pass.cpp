/*****************************************************
 * Partial Class: Renderer
 * Members: Frame - Render Pass (Private)
 *****************************************************/

#include "Renderer.hpp"

using namespace sdfRay4d;

void Renderer::cmdRenderPass()
{
  auto windowSize = m_vkWindow->swapChainImageSize();
  auto extentWidth = uint32_t(windowSize.width());
  auto extentHeight = uint32_t(windowSize.height());

  ClearColor clearColor = {
    {
      0.67f,
      0.84f,
      0.9f,
      1.0f
    }
  };
  ClearDepthStencil clearDepthStencil = { 1, 0 };
  Clear clearVals[3];
  clearVals[0].color = clearVals[2].color = clearColor;
  clearVals[1].depthStencil = clearDepthStencil;

  RenderPassBeginInfo renderPassInfo = {}; // memset
  renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
  renderPassInfo.renderPass = m_vkWindow->defaultRenderPass();
  renderPassInfo.framebuffer = m_vkWindow->currentFramebuffer();
  renderPassInfo.renderArea.extent.width = extentWidth;
  renderPassInfo.renderArea.extent.height = extentHeight;
  renderPassInfo.clearValueCount = m_vkWindow->sampleCountFlagBits() > VK_SAMPLE_COUNT_1_BIT ? 3 : 2;
  renderPassInfo.pClearValues = clearVals;

  auto cmdBuffer = m_vkWindow->currentCommandBuffer();

  m_deviceFuncs->vkCmdBeginRenderPass(cmdBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

    Viewport viewport = {
      0.0f, // x
      0.0f, // y
      float(extentWidth), // width
      float(extentHeight), // height
      0, // minDepth
      1 // maxDepth
    };
    m_deviceFuncs->vkCmdSetViewport(cmdBuffer, 0, 1, &viewport);

    Rect2D scissor = {
      { // offset
        0, // x
        0 // y
      },
      {  // extent
        extentWidth, // width
        extentHeight // height
      }
    };
    m_deviceFuncs->vkCmdSetScissor(cmdBuffer, 0, 1, &scissor);

    buildDrawCalls(m_sdfMaterial, cmdBuffer, windowSize);

  m_deviceFuncs->vkCmdEndRenderPass(cmdBuffer);
}
