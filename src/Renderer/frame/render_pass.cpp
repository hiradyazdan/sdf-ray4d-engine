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

  RenderPassBeginInfo renderPassBeginInfo = {}; // memset
  renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
  renderPassBeginInfo.renderPass = m_pipelineHelper.getRenderPass();
  renderPassBeginInfo.framebuffer = m_vkWindow->currentFramebuffer();
  renderPassBeginInfo.renderArea.extent.width = extentWidth;
  renderPassBeginInfo.renderArea.extent.height = extentHeight;
  renderPassBeginInfo.clearValueCount = m_vkWindow->sampleCountFlagBits() > VK_SAMPLE_COUNT_1_BIT ? 3 : 2;
  renderPassBeginInfo.pClearValues = clearVals;

  auto cmdBuffer = m_vkWindow->currentCommandBuffer();

  m_deviceFuncs->vkCmdBeginRenderPass(
    cmdBuffer,
    &renderPassBeginInfo,
    VK_SUBPASS_CONTENTS_INLINE
  );

    cmdSetViewportAndScissor(
      cmdBuffer,
      extentWidth, extentHeight
    );

    createSDFDrawCalls(
      m_sdfrMaterial,
      cmdBuffer,
      (float) extentWidth, (float) extentHeight
    );
    createActorDrawCalls(
      m_actorMaterial,
      cmdBuffer
    );

  m_deviceFuncs->vkCmdEndRenderPass(cmdBuffer);
}

void Renderer::cmdSetViewportAndScissor(
  CmdBuffer &_cmdBuffer,
  uint32_t _extentWidth,
  uint32_t _extentHeight
)
{
  Viewport viewport = {
    0.0f, // x
    0.0f, // y
    float(_extentWidth), // width
    float(_extentHeight), // height
    0, // minDepth
    1 // maxDepth
  };
  m_deviceFuncs->vkCmdSetViewport(
    _cmdBuffer,
    0,
    1,
    &viewport
  );

  Rect2D scissor = {
    { // offset
      0, // x
      0 // y
    },
    {  // extent
      _extentWidth, // width
      _extentHeight // height
    }
  };
  m_deviceFuncs->vkCmdSetScissor(
    _cmdBuffer,
    0,
    1,
    &scissor
  );
}
