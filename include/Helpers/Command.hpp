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
      );

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
      );

    /**
     * Command Execution Functions (PUBLIC)
     * -------------------------------------------------
     *
     */
    public:
      void executeRenderPass(const std::vector<MaterialPtr> &_materials);
      void executePipelineBarrier(const MaterialPtr &_material);

    /**
     * Command Execution Functions (PRIVATE)
     * -------------------------------------------------
     *
     */
    private:
      void executeCmdSetViewport();
      void executeCmdSetScissor();
      void executeCmdBind          (const MaterialPtr &_material);
      void executeCmdPushConstants (const MaterialPtr &_material);
      void executeCmdDraw          (const MaterialPtr &_material);

    private:
      void setRenderPassHelper(const RenderPassHelper &_renderPassHelper);

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
