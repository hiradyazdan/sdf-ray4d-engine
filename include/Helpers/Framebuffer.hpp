#pragma once

#include "BaseHelper.hpp"

namespace sdfRay4d::helpers
{
  /**
   * @class FramebufferHelper
   * @brief A helper class for framebuffer creation and usage
   *
   * @example
   *
   */
  class FramebufferHelper : protected BaseHelper
  {
    friend class PipelineHelper;
    friend class RenderPassHelper;
    friend class CommandHelper;

    public:
      FramebufferHelper(
        const Device &_device,
        QVulkanDeviceFunctions *_deviceFuncs,
        texture::ImageView *_attachments
      );

    /**
     * @note FramebufferHelper is non-copyable
     */
    public:
      FramebufferHelper() = default;
      FramebufferHelper(const FramebufferHelper&) = delete;

    private:
      void createFramebuffer(const RenderPass &_renderPass);
      void setSize(uint32_t _extentWidth, uint32_t _extentHeight);
      void setDefaultFramebuffer(const Framebuffer &_framebuffer);
      Framebuffer &getFramebuffer(
        const RenderPass &_renderPass,
        bool _useDefault
      );

    private:
      Device m_device = VK_NULL_HANDLE;
      QVulkanDeviceFunctions *m_deviceFuncs = VK_NULL_HANDLE;

      Framebuffer m_defaultFrameBuffer = VK_NULL_HANDLE;
      Framebuffer m_frameBuffer = VK_NULL_HANDLE;
      texture::ImageView *m_attachments = VK_NULL_HANDLE;

      uint32_t m_extentWidth = 0;
      uint32_t m_extentHeight = 0;
  };
}
