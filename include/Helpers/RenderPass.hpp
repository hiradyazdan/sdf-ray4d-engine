#pragma once

#include "Framebuffer.hpp"

namespace sdfRay4d::helpers
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

    public:
      RenderPassHelper(
        const Device &_device,
        QVulkanDeviceFunctions *_deviceFuncs,
        const SampleCountFlags &_sampleCountFlags,
        const RenderPass &_renderPass
      );

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
      void createCustomRenderPass(
        const Format &_colorFormat = VK_FORMAT_B8G8R8A8_UNORM,
        const Format &_depthStencilFormat = VK_FORMAT_D16_UNORM//VK_FORMAT_D24_UNORM_S8_UINT
      );
      void createRenderPassInfo(const std::vector<MaterialPtr> &_materials);
      RenderPass &getRenderPass(bool _useDefault = true);
      RenderPassBeginInfo &getRenderPassInfo() { return m_renderPassInfo; }

    /**
     * Framebuffer Helpers
     * -------------------------------------------------
     */
    private:
      texture::ImageView getFramebufferAttachments();
      void setFramebufferAttachments(const texture::ImageView &_fbAttachments);
      void setDefaultFramebuffer(const Framebuffer &_framebuffer);
      void setFramebufferSize(uint32_t _extentWidth, uint32_t _extentHeight);


    private:
      FramebufferHelper m_framebufferHelper; // friend

      Device m_device = VK_NULL_HANDLE;
      QVulkanDeviceFunctions *m_deviceFuncs = VK_NULL_HANDLE;

      RenderPass m_defaultRenderPass = VK_NULL_HANDLE;
      RenderPass m_renderPass = VK_NULL_HANDLE;

      SampleCountFlags m_sampleCountFlags = {};
      Clear m_clearValues[3] = {};
      RenderPassBeginInfo m_renderPassInfo = {};

      uint32_t m_extentWidth = 0;
      uint32_t m_extentHeight = 0;
  };
}
