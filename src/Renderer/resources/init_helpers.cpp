/*****************************************************
 * Partial Class: Renderer
 * Members: Init Resources Helpers (Private)
 *****************************************************/

#include "Renderer.hpp"

using namespace sdfRay4d;

void Renderer::initVkFunctions()
{
  auto &&vkInstance = m_vkWindow->vulkanInstance();
  m_device          = m_vkWindow->device();
  m_deviceFuncs     = vkInstance->deviceFunctions(m_device);

  m_pipelineHelper.initHelpers(
    m_device,
    m_deviceFuncs,
    m_isMSAA ? m_vkWindow->sampleCountFlagBits() : VK_SAMPLE_COUNT_1_BIT,
    m_vkWindow->defaultRenderPass()
  );
}

void Renderer::initMaterials()
{
  initDepthMaterial();
  initActorMaterial();
  initSDFRMaterial();
}

void Renderer::initShaders()
{
  initDepthShaders();
  initActorShaders();
  initSDFRShaders();
}

const VkPhysicalDeviceLimits *Renderer::getDeviceLimits() const
{
  return &m_vkWindow->physicalDeviceProperties()->limits;
}

device::Size Renderer::setDynamicOffsetAlignment(
  device::Size _offset
)
{
  const auto &minUBOAlignment = getDeviceLimits()->minUniformBufferOffsetAlignment;

  return (_offset + minUBOAlignment - 1) & ~(minUBOAlignment - 1);
}

/**
 * @brief creates depth image view (texture)
 * to be used for depth/z-buffer calculation and use between passes
 */
void Renderer::createDepthView()
{
  auto &depthTexture = m_depthMaterial->texture;
  auto &depthView = depthTexture.getImageView();
  const auto &aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;

  if(depthView) return;

  depthTexture.createImage(
    m_windowSize.width(),
    m_windowSize.height(),
    VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT
    | VK_IMAGE_USAGE_SAMPLED_BIT
  );
  depthTexture.createImageMemory(m_vkWindow->deviceLocalMemoryIndex());
  depthTexture.createImageMemoryBarrier(
    VK_IMAGE_LAYOUT_UNDEFINED,
    VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL,
    aspectMask,
    VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT,
    VK_ACCESS_SHADER_READ_BIT
  );
  depthTexture.createImageView(
    aspectMask,
    depthView
  );

  m_pipelineHelper.setFramebufferAttachments({
    depthTexture.getImageView() // framebuffer depth attachment for custom renderPass
  });
}
