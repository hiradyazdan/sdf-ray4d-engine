#pragma once

#include "BaseHelper.hpp"
#include "RenderPass.hpp"

namespace sdfRay4d::helpers
{
  /**
   * @class CommandHelper
   * @brief
   *
   * @example
   *
   */
  class CommandHelper : protected BaseHelper
  {
    friend class PipelineHelper;

    public:
      CommandHelper(
        const Device &_device,
        QVulkanDeviceFunctions *_deviceFuncs
      ) noexcept;

    /**
     * @note CommandHelper is non-copyable
     */
    public:
      CommandHelper() = default;
      CommandHelper(const CommandHelper&) = delete;

    public:
      void init(
        const CmdBuffer &_cmdBuffer,
        const Framebuffer &_framebuffer,
        int _frameId,
        uint32_t _extentWidth,
        uint32_t _extentHeight
      ) noexcept;

    /**
     * Command Execution Functions (PUBLIC)
     * -------------------------------------------------
     *
     */
    public:
      void executeRenderPass(const std::vector<MaterialPtr> &_materials) noexcept;
      void executePipelineBarrier(const MaterialPtr &_material) noexcept;

    /**
     * Command Execution Functions (PRIVATE)
     * -------------------------------------------------
     *
     */
    private:
      void executeCmdSetViewport() noexcept;
      void executeCmdSetScissor() noexcept;
      void executeCmdBind          (const MaterialPtr &_material) noexcept;
      void executeCmdPushConstants (const MaterialPtr &_material) noexcept;
      void executeCmdDraw          (const MaterialPtr &_material) noexcept;

    private:
      void setRenderPassHelper(const RenderPassHelper &_renderPassHelper) noexcept;

    private:
      Device m_device = VK_NULL_HANDLE;
      QVulkanDeviceFunctions *m_deviceFuncs = VK_NULL_HANDLE;

      CmdBuffer m_cmdBuffer = VK_NULL_HANDLE;

      RenderPassHelper m_renderPassHelper;

      int m_frameId = 0;
      uint32_t m_extentWidth = 0;
      uint32_t m_extentHeight = 0;
  };
}
