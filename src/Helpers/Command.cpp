/*****************************************************
 * Partial Class: CommandHelper (Ctor & General)
 * Members: General Functions (Public)
 *
 * This Class is split into partials to categorize
 * and classify the functionality
 * for the purpose of readability/maintainability
 *
 * The partials can be found in the respective
 * directory named as the class name
 *
 * Partials:
 * - bindings.cpp
 * - draw_calls.cpp
 *****************************************************/

#include "Helpers/Command.hpp"

using namespace sdfRay4d::helpers;

/**
 *
 * @param[in] _device
 * @param[in] _deviceFuncs
 */
CommandHelper::CommandHelper(
  const Device &_device,
  QVulkanDeviceFunctions *_deviceFuncs
) :
  m_device(_device)
, m_deviceFuncs(_deviceFuncs)
{}

/**
 *
 * @param[in] _cmdBuffer
 * @param[in] _framebuffer default Qt Vulkan framebuffer
 * @param[in] _frameId
 * @param[in] _extentWidth
 * @param[in] _extentHeight
 */
void CommandHelper::init(
  const CmdBuffer &_cmdBuffer,
  const Framebuffer &_framebuffer,
  int _frameId,
  uint32_t _extentWidth,
  uint32_t _extentHeight
)
{
  m_cmdBuffer     = _cmdBuffer;
  m_frameId       = _frameId;
  m_extentWidth   = _extentWidth;
  m_extentHeight  = _extentHeight;

  m_renderPassHelper.setDefaultFramebuffer(_framebuffer);
  m_renderPassHelper.setFramebufferSize(
    _extentWidth,
    _extentHeight
  );

  executeCmdSetViewport();
  executeCmdSetScissor();
}

/**
 *
 * @param[in] _renderPassHelper
 */
void CommandHelper::setRenderPassHelper(const RenderPassHelper &_renderPassHelper)
{
  m_renderPassHelper = _renderPassHelper;
}

void CommandHelper::executeCmdSetViewport()
{
  Viewport viewport = {
    0.0f, // x
    0.0f, // y
    float(m_extentWidth), // width
    float(m_extentHeight), // height
    0, // minDepth
    1 // maxDepth
  };
  m_deviceFuncs->vkCmdSetViewport(
    m_cmdBuffer,
    0,
    1,
    &viewport
  );
}

void CommandHelper::executeCmdSetScissor()
{
  Rect2D scissor = {
    { // offset
      0, // x
      0 // y
    },
    {  // extent
      m_extentWidth, // width
      m_extentHeight // height
    }
  };
  m_deviceFuncs->vkCmdSetScissor(
    m_cmdBuffer,
    0,
    1,
    &scissor
  );
}

/**
 *
 * @param[in] _material
 */
void CommandHelper::executeCmdPushConstants(const MaterialPtr &_material)
{
  if(_material->pushConstantRangeCount <= 0) return;

  /**
   * @note this needs to be local and a copy
   * than reference the original, to avoid
   * adding infinite number of per frame data
   *
   * TODO: Fix dynamic data/mouse positions
   */
  auto pushConstants = _material->pushConstants;

  float perFrameData[3] = {
    1, // mouse position x
    1, // mouse position y

    1 // time
  };

  pushConstants.insert(
    pushConstants.end(), std::begin(perFrameData), std::end(perFrameData)
  );

  m_deviceFuncs->vkCmdPushConstants(
    m_cmdBuffer,
    _material->pipelineLayout,
    VK_SHADER_STAGE_FRAGMENT_BIT,
    0/*sizeof(mvp) - 4*/,
    sizeof(pushConstants.data()),
    pushConstants.data()
  );
}

/**
 *
 * @param[in] _materials
 */
void CommandHelper::executeRenderPass(
  const std::vector<MaterialPtr> &_materials
)
{
  m_renderPassHelper.createRenderPassInfo(_materials);

  m_deviceFuncs->vkCmdBeginRenderPass(
    m_cmdBuffer,
    &m_renderPassHelper.getRenderPassInfo(),
    VK_SUBPASS_CONTENTS_INLINE
  );

    for(const auto &material : _materials)
    {
      executeCmdBind(material);
      executeCmdPushConstants(material);
      executeCmdDraw(material);
    }

  m_deviceFuncs->vkCmdEndRenderPass(m_cmdBuffer);
}

/**
 *
 * @param[in] _material
 */
void CommandHelper::executePipelineBarrier(const MaterialPtr &_material)
{
  /**
   * @note GPU - GPU (device) Sync Command
   */
  m_deviceFuncs->vkCmdPipelineBarrier(
    m_cmdBuffer,
    _material->sourceStage,
    _material->destinationStage,
    0,
    0,
    nullptr, // global
    0,
    nullptr, // buffer device memory
    1,
    &_material->texture.getImageMemoryBarrier() // image device memory
  );
}
