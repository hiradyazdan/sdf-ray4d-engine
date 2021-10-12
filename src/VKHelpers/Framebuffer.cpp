/*****************************************************
 * Class: FramebufferHelper (General)
 * Members: General Functions (Public/Private)
 * Partials: None
 *****************************************************/

#include "VKHelpers/Framebuffer.hpp"

using namespace sdfRay4d::vkHelpers;

/**
 * @param[in] _device
 * @param[in] _deviceFuncs
 */
FramebufferHelper::FramebufferHelper(
  const Device &_device,
  QVulkanDeviceFunctions *_deviceFuncs
) noexcept :
  m_device(_device)
, m_deviceFuncs(_deviceFuncs)
{}

/**
 *
 * @param[in] _attachments
 */
void FramebufferHelper::setAttachments(
  const ImageViewList &_attachments
) noexcept
{
  m_attachments = _attachments;
}

/**
 * @brief sets default Qt Vulkan framebuffer
 * @param[in] _framebuffer
 */
void FramebufferHelper::setDefaultFramebuffer(
  const Framebuffer &_framebuffer
) noexcept
{
  m_defaultFrameBuffer = _framebuffer;
}

/**
 * @brief sets framebuffer width and height
 * @param[in] _extentWidth
 * @param[in] _extentHeight
 */
void FramebufferHelper::setSize(
  uint32_t _extentWidth,
  uint32_t _extentHeight
) noexcept
{
  m_extentWidth = _extentWidth;
  m_extentHeight = _extentHeight;
}

/**
 * @brief retrieves current in-use framebuffer instance
 * @param[in] _renderPass
 * @param[in] _useDefault
 * @return Framebuffer instance
 */
Framebuffer &FramebufferHelper::getFramebuffer(
  const RenderPass &_renderPass,
  bool _useDefault
) noexcept
{
  /**
   * @note
   * Since this method is called per frame,
   * to make framebuffer instance reusable/non-copyable
   * and so to be able to draw to it per frame,
   * like a singleton instance
   */
  if(!m_frameBuffer && !_useDefault)
  {
    createFramebuffer(_renderPass);
  }

  /**
   * @note
   * if useDefault  => already populated with Qt default framebuffer with command helper's init function in render loop
   * else           => custom framebuffer is created and populated by above
   */
  return _useDefault ? m_defaultFrameBuffer : m_frameBuffer;
}

/**
 * @brief creates a custom (non Qt-Vulkan) framebuffer
 * @param[in] _renderPass
 */
void FramebufferHelper::createFramebuffer(
  const RenderPass &_renderPass
) noexcept
{
  if(m_extentWidth == 0 || m_extentHeight == 0)
  {
    qWarning("Framebuffer width or height is not set!");

    return;
  }

  FramebufferInfo framebufferInfo = {}; // memset

  framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
  framebufferInfo.renderPass = _renderPass; // custom renderPass
  framebufferInfo.attachmentCount = m_attachments.size(); // depth pass attachment image view is only 1
  framebufferInfo.pAttachments = m_attachments.data();
  framebufferInfo.width = m_extentWidth;
  framebufferInfo.height = m_extentHeight;
  framebufferInfo.layers = 1;

  auto result = m_deviceFuncs->vkCreateFramebuffer(
    m_device,
    &framebufferInfo,
    nullptr,
    &m_frameBuffer
  );

  if (result != VK_SUCCESS)
  {
    qWarning("Failed to create Framebuffer: %d", result);
  }
}
