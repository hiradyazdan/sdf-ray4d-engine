/*****************************************************
 * Partial Class: Renderer
 * Members: Frame - Render Pass (Private)
 *****************************************************/

#include "Renderer.hpp"

using namespace sdfRay4d;

/**
 * TODO:
 * Clean up this and port the main parts
 * to RenderPass Helper
 */
void Renderer::cmdRenderPass()
{
  auto extentWidth = uint32_t(m_windowSize.width());
  auto extentHeight = uint32_t(m_windowSize.height());

  ClearColor clearColor = {
    {
      0,//0.67f,
      0,//0.84f,
      0,//0.9f,
      1.0f
    }
  };
  ClearDepthStencil clearDepthStencil = { 1, 0 };
  Clear clearVals[3];
  clearVals[0].color = clearVals[2].color = clearColor;
  clearVals[1].depthStencil = clearDepthStencil;

  RenderPassBeginInfo renderPassBeginInfo = {}; // memset

  renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
  renderPassBeginInfo.renderPass = m_depthMaterial->renderPass;

  renderPassBeginInfo.framebuffer = m_pipelineHelper.getFramebuffer(extentWidth, extentHeight);
  renderPassBeginInfo.renderArea.extent.width = extentWidth;
  renderPassBeginInfo.renderArea.extent.height = extentHeight;
  renderPassBeginInfo.clearValueCount = m_vkWindow->sampleCountFlagBits() > VK_SAMPLE_COUNT_1_BIT ? 3 : 2;
  renderPassBeginInfo.pClearValues = clearVals;

  auto cmdBuffer = m_vkWindow->currentCommandBuffer();
  m_vkWindow->graphicsCommandPool();

  m_deviceFuncs->vkCmdBeginRenderPass(
    cmdBuffer,
    &renderPassBeginInfo,
    VK_SUBPASS_CONTENTS_INLINE
  );

    cmdSetViewportAndScissor(
      cmdBuffer,
      extentWidth, extentHeight
    );

    createDepthDrawCalls(
      m_depthMaterial,
      cmdBuffer
    );

  m_deviceFuncs->vkCmdEndRenderPass(cmdBuffer);

  texture::ImageMemoryBarrier barrier = {};

  barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
  barrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
  barrier.newLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL;
  barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
  barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
  barrier.image = m_depthMaterial->texture.getImage();
  barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
  barrier.subresourceRange.baseMipLevel = 0;
  barrier.subresourceRange.levelCount = 1;
  barrier.subresourceRange.baseArrayLayer = 0;
  barrier.subresourceRange.layerCount = 1;
  barrier.srcAccessMask = 0;
  barrier.dstAccessMask = 0;

  pipeline::StageFlags sourceStage;
  pipeline::StageFlags destinationStage;

  barrier.srcAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
  barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
  sourceStage = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
  destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;

  cmdBuffer = m_vkWindow->currentCommandBuffer();

  /**
   * GPU - GPU (device) Sync
   */
  m_deviceFuncs->vkCmdPipelineBarrier(
    cmdBuffer,
    sourceStage,
    destinationStage,
    0,
    0,
    nullptr,
    0,
    nullptr,
    1,
    &barrier
  );

  extentWidth = uint32_t(m_windowSize.width());
  extentHeight = uint32_t(m_windowSize.height());

  clearColor = {
    {
      0.67f,
      0.84f,
      0.9f,
      1.0f
    }
  };
  clearDepthStencil = { 1, 0 };
  clearVals[0].color = clearVals[2].color = clearColor;
  clearVals[1].depthStencil = clearDepthStencil;

  renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
  renderPassBeginInfo.renderPass = m_sdfrMaterial->renderPass; // default Qt Vulkan RenderPass
  renderPassBeginInfo.framebuffer = m_vkWindow->currentFramebuffer();
  renderPassBeginInfo.renderArea.extent.width = extentWidth;
  renderPassBeginInfo.renderArea.extent.height = extentHeight;
  renderPassBeginInfo.clearValueCount = m_vkWindow->sampleCountFlagBits() > VK_SAMPLE_COUNT_1_BIT ? 3 : 2;
  renderPassBeginInfo.pClearValues = clearVals;

  cmdBuffer = m_vkWindow->currentCommandBuffer();

  m_deviceFuncs->vkCmdBeginRenderPass(
    cmdBuffer,
    &renderPassBeginInfo,
    VK_SUBPASS_CONTENTS_INLINE
  );
    createActorDrawCalls(
      m_actorMaterial,
      cmdBuffer
    );

    createSDFRDrawCalls(
      m_sdfrMaterial,
      cmdBuffer,
      (float) extentWidth, (float) extentHeight
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
