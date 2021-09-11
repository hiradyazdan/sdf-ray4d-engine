/*****************************************************
 * Partial Class: Renderer
 * Members: Swapchain Helpers (Private)
 *****************************************************/

#include "Renderer.hpp"

using namespace sdfRay4d;

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
    VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT |
    VK_IMAGE_USAGE_SAMPLED_BIT
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
}
