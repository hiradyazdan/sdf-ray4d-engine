#pragma once

#include "Framebuffer.hpp"

namespace sdfRay4d::vkHelpers
{
  /**
   * @class RenderPassHelper
   * @brief
   *
   * @example
   *
   */
  class RenderPassHelper : protected BaseHelper
  {
    friend class PipelineHelper;
    friend class CommandHelper;

    using ImageViewList = std::vector<image::View>;

    public:
      RenderPassHelper(
        const device::Device &_device,
        QVulkanDeviceFunctions *_deviceFuncs,
        const image::SampleCountFlagBits &_sampleCountFlags,
        const renderpass::RenderPass &_renderPass
      ) noexcept;

    /**
     * @note RenderPassHelper is non-copyable
     */
    public:
      RenderPassHelper() = default;
      RenderPassHelper(const RenderPassHelper&) = delete;

    /**
     * Create RenderPass Helpers
     * -------------------------------------------------
     */
    private:
      void createRenderPass(
        const Format &_colorFormat = VK_FORMAT_B8G8R8A8_UNORM,
        const Format &_depthStencilFormat = VK_FORMAT_D16_UNORM//VK_FORMAT_D24_UNORM_S8_UINT
      ) noexcept;
      void createBeginInfo(const std::vector<MaterialPtr> &_materials) noexcept;
      renderpass::RenderPass &getRenderPass(bool _useDefault = true) noexcept;
      renderpass::BeginInfo &getBeginInfo() noexcept { return m_beginInfo; }

    /**
     * Framebuffer Helpers
     * -------------------------------------------------
     */
    private:
      inline ImageViewList &getFramebufferAttachments() noexcept { return m_framebufferHelper.getAttachments(); }
      void setFramebufferAttachments(const ImageViewList &_fbAttachments) noexcept;
      void setDefaultFramebuffer(const framebuffer::Framebuffer &_framebuffer) noexcept;
      void setFramebufferSize(uint32_t _extentWidth, uint32_t _extentHeight) noexcept;

    private:
      FramebufferHelper m_framebufferHelper; // friend

      device::Device m_device                         = VK_NULL_HANDLE;
      QVulkanDeviceFunctions *m_deviceFuncs           = VK_NULL_HANDLE;

      renderpass::RenderPass m_defaultRenderPass      = VK_NULL_HANDLE;
      renderpass::RenderPass m_renderPass             = VK_NULL_HANDLE;

      image::SampleCountFlagBits m_sampleCountFlags = {};
      Clear m_clearValues[3]                          = {};
      renderpass::BeginInfo m_beginInfo               = {};

      uint32_t m_extentWidth                          = 0;
      uint32_t m_extentHeight                         = 0;
  };
}
