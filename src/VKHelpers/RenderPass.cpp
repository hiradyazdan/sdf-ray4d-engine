/*****************************************************
 * Class: RenderPassHelper (General)
 * Members: General Functions (Public/Private)
 *
 * This Class is split into partials to categorize
 * and classify the functionality
 * for the purpose of readability/maintainability
 *
 * The partials can be found in the respective
 * directory named as the class name
 *
 * Partials:
 * - set_framebuffer_helpers.cpp
 *****************************************************/

#include "VKHelpers/RenderPass.hpp"

using namespace sdfRay4d::vkHelpers;

/**
 *
 * @param[in] _device
 * @param[in] _deviceFuncs device functions
 * @param[in] _sampleCountFlags
 * @param[in] _renderPass
 */
RenderPassHelper::RenderPassHelper(
  const device::Device &_device,
  QVulkanDeviceFunctions *_deviceFuncs,
  const image::SampleCountFlagBits &_sampleCountFlags,
  const renderpass::RenderPass &_renderPass
) noexcept :
  m_device(_device)
, m_deviceFuncs(_deviceFuncs)
, m_sampleCountFlags(_sampleCountFlags)
, m_defaultRenderPass(_renderPass)
, m_framebufferHelper(FramebufferHelper(_device, _deviceFuncs))
{}

/**
 * @brief creates a custom (non Qt-Vulkan) renderPass
 *
 * @note
 * The framebuffer has two attachments (color, depth-stencil) when
 * multisampling is not in use, and three (color resolve, depth-stencil,
 * multisample color) when sampleCountFlagBits() is greater than
 * {VK_SAMPLE_COUNT_1_BIT}. Renderers must take this into account, for
 * example when providing clear values.
 *
 * @param[in] _colorFormat
 * @param[in] _depthStencilFormat
 * @param[out] m_renderPass
 */
void RenderPassHelper::createRenderPass(
  const Format &_colorFormat,
  const Format &_depthStencilFormat
) noexcept
{
  renderpass::AttachmentDesc attDesc[1] = {}; // memset

  attDesc[0].format = _depthStencilFormat;
  attDesc[0].samples = m_sampleCountFlags;
  attDesc[0].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
  attDesc[0].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
  attDesc[0].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
  attDesc[0].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
  attDesc[0].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
  attDesc[0].finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

  renderpass::AttachmentRef dsRef = {
    0,
    VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL
  };

  renderpass::SubpassDesc subPassDesc = {}; // memset
  subPassDesc.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
  subPassDesc.colorAttachmentCount = 0;
  subPassDesc.pColorAttachments = nullptr;
  subPassDesc.pDepthStencilAttachment = &dsRef;

  renderpass::Info renderPassInfo = {}; // memset
  renderPassInfo.sType = renderpass::StructureType::RENDER_PASS_INFO;
  renderPassInfo.attachmentCount = 1;
  renderPassInfo.pAttachments = attDesc;
  renderPassInfo.subpassCount = 1;
  renderPassInfo.pSubpasses = &subPassDesc;

  auto result = m_deviceFuncs->vkCreateRenderPass(
    m_device,
    &renderPassInfo,
    nullptr,
    &m_renderPass
  );

  if (result != VK_SUCCESS)
  {
    qFatal("Failed to create RenderPass: %d", result);
  }
}

/**
 * @brief creates renderPassBeginInfo for both default and custom renderPass
 * @param[in] _materials
 */
void RenderPassHelper::createBeginInfo(
  const std::vector<MaterialPtr> &_materials
) noexcept
{
  const auto &rpMaterial = _materials[0];
  const auto &rpFramebuffer = m_framebufferHelper.getFramebuffer(
    rpMaterial->renderPass,
    rpMaterial->isDefault
  );

  for (const auto &material : _materials)
  {
    material->framebuffer = rpFramebuffer;
  }

  ClearColor clearColor = {
    {
      0,//0.67f,
      0,//0.84f,
      0,//0.9f,
      1.0f
    }
  };
  ClearDepthStencil clearDepthStencil = { 1, 0 };

  m_clearValues[0].color = m_clearValues[2].color = clearColor;
  m_clearValues[1].depthStencil = clearDepthStencil;

  m_beginInfo.sType = renderpass::StructureType::RENDER_PASS_BEGIN_INFO;
  m_beginInfo.renderPass = rpMaterial->renderPass;
  m_beginInfo.framebuffer = rpMaterial->framebuffer;
  m_beginInfo.renderArea.extent.width = m_extentWidth;
  m_beginInfo.renderArea.extent.height = m_extentHeight;
  m_beginInfo.clearValueCount = m_sampleCountFlags > VK_SAMPLE_COUNT_1_BIT ? 3 : 2;
  m_beginInfo.pClearValues = m_clearValues;
}

/**
 *
 * @brief retrieves material's current/in-use renderPass instance
 * @param[in] _useDefault
 * @return RenderPass instance
 */
renderpass::RenderPass &RenderPassHelper::getRenderPass(bool _useDefault) noexcept
{
  if(!m_renderPass && !_useDefault)
  {
    createRenderPass();
  }

  /**
   * @note
   * if useDefault  => already populated with Qt default renderPass in ctor
   * else           => custom renderPass is created and populated by above
   */
  return _useDefault ? m_defaultRenderPass : m_renderPass;
}
