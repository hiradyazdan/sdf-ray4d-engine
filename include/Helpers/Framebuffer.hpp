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
    friend class RenderPassHelper;
    friend class CommandHelper;

    using ImageViewList = std::vector<texture::ImageView>;

    /**
     * @note FramebufferHelper is non-copyable
     */
    public:
      FramebufferHelper() = default;
      FramebufferHelper(const FramebufferHelper&) = delete;

    private:
      FramebufferHelper(
        const Device &_device,
        QVulkanDeviceFunctions *_deviceFuncs
      ) noexcept;

    private:
      void createFramebuffer(const RenderPass &_renderPass) noexcept;
      void setAttachments(const ImageViewList &_attachments) noexcept;
      void setSize(uint32_t _extentWidth, uint32_t _extentHeight) noexcept;
      void setDefaultFramebuffer(const Framebuffer &_framebuffer) noexcept;
      ImageViewList &getAttachments() noexcept { return m_attachments; }
      Framebuffer &getFramebuffer(
        const RenderPass &_renderPass,
        bool _useDefault
      ) noexcept;

    private:
      Device m_device = VK_NULL_HANDLE;
      QVulkanDeviceFunctions *m_deviceFuncs = VK_NULL_HANDLE;

      Framebuffer m_defaultFrameBuffer = VK_NULL_HANDLE;
      Framebuffer m_frameBuffer = VK_NULL_HANDLE;
      ImageViewList m_attachments = {};

      uint32_t m_extentWidth = 0;
      uint32_t m_extentHeight = 0;
  };
}
