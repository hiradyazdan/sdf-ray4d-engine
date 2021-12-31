/*****************************************************
 * Partial Class: RenderPassHelper
 * Members: Set Framebuffer Helpers (Private)
 *****************************************************/

#include "VKHelpers/RenderPass.hpp"

using namespace sdfRay4d::vkHelpers;

RenderPassHelper::ImageViewList &RenderPassHelper::getFramebufferAttachments() noexcept
{
  return m_framebufferHelper.getAttachments();
}

/**
 *
 * @param[in] _fbAttachments
 */
void RenderPassHelper::setFramebufferAttachments(
  const ImageViewList &_fbAttachments
) noexcept
{
  m_framebufferHelper.setAttachments(_fbAttachments);
}

/**
 *
 * @param[in] _framebuffer
 */
void RenderPassHelper::setDefaultFramebuffer(
  const framebuffer::Framebuffer &_framebuffer
) noexcept
{
  m_framebufferHelper.setDefaultFramebuffer(_framebuffer);
}

/**
 *
 * @param[in] _extentWidth
 * @param[in] _extentHeight
 */
void RenderPassHelper::setFramebufferSize(
  uint32_t _extentWidth,
  uint32_t _extentHeight
) noexcept
{
  m_extentWidth = _extentWidth;
  m_extentHeight = _extentHeight;

  m_framebufferHelper.setSize(
    _extentWidth,
    _extentHeight
  );
}
