#pragma once

#include "Types.hpp"
#include "Material.hpp"

namespace sdfRay4d
{
  using namespace vk;

  /**
   * @class RenderPassHelper
   * @brief
   *
   * @example
   *
   */
  class RenderPassHelper
  {
    friend class PipelineHelper;

    using MaterialPtr = std::shared_ptr<Material>;

    public:
      RenderPassHelper(
        const Device &_device,
        QVulkanDeviceFunctions *_deviceFuncs,
        const SampleCountFlags &_sampleCountFlags,
        const RenderPass &_renderPass,
        bool _useDefault = true
      );

    /**
     * @note RenderPassHelper is non-copyable
     */
    public:
      RenderPassHelper() = default;
      RenderPassHelper(const RenderPassHelper&)=delete;

    public:
      void createCustomRenderPass(
        const Format &_colorFormat = VK_FORMAT_B8G8R8A8_UNORM,
        const Format &_depthStencilFormat = VK_FORMAT_D24_UNORM_S8_UINT
      );
      void setCmdBuffer(CmdBuffer &_cmdBuffer);
      void cmdSetViewportAndScissor(
        CmdBuffer &_cmdBuffer,
        uint32_t _extentWidth,
        uint32_t _extentHeight
      ); // TODO: Rename
      void createSDFDrawCalls(
        const MaterialPtr &_material,
        CmdBuffer &_cmdBuffer,
        float _extentWidth,
        float _extentHeight
      );
      void createActorDrawCalls(
        const MaterialPtr &_material,
        CmdBuffer &_cmdBuffer
      );
      void cmdRenderPass();

    private:
      /**
       * @note this method should only be available to PipelineHelper
       * as RenderPass instance should not be accessed independently.
       * @return RenderPass instance
       */
      RenderPass getRenderPass() { return m_customRenderPass; }

    private:
      Device m_device = VK_NULL_HANDLE;
      QVulkanDeviceFunctions *m_deviceFuncs = VK_NULL_HANDLE;

      SampleCountFlags m_sampleCountFlags;

      RenderPass m_customRenderPass = VK_NULL_HANDLE;
  };
}
