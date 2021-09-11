/*****************************************************
 * Partial Class: RenderPassHelper
 * Members: Set Framebuffer Helpers (Private)
 *****************************************************/

#include "Helpers/RenderPass.hpp"

using namespace sdfRay4d::helpers;

texture::ImageView RenderPassHelper::getFramebufferAttachments()
{
  return m_framebufferHelper.getAttachments();
}

/**
 *
 * @param[in] _fbAttachments
 */
void RenderPassHelper::setFramebufferAttachments(
  const texture::ImageView &_fbAttachments
)
{
  m_framebufferHelper.setAttachments(_fbAttachments);
}

/**
 *
 * @param[in] _framebuffer
 */
void RenderPassHelper::setDefaultFramebuffer(
  const Framebuffer &_framebuffer
)
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
)
{
  m_extentWidth = _extentWidth;
  m_extentHeight = _extentHeight;

  m_framebufferHelper.setSize(
    _extentWidth,
    _extentHeight
  );
}
