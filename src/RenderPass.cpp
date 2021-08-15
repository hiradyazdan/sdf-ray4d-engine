/*****************************************************
 * Class: RenderPassHelper (General)
 * Members: General Functions (Public/Private)
 * Partials: None
 *****************************************************/

#include "RenderPass.hpp"

using namespace sdfRay4d;

RenderPassHelper::RenderPassHelper(
  const Device &_device,
  QVulkanDeviceFunctions *_deviceFuncs,
  const SampleCountFlags &_sampleCountFlags,
  const RenderPass &_renderPass
) :
  m_device(_device)
  , m_deviceFuncs(_deviceFuncs)
  , m_sampleCountFlags(_sampleCountFlags)
{}

/**
 * @brief
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
void RenderPassHelper::createCustomRenderPass(
  const Format &_colorFormat,
  const Format &_depthStencilFormat
)
{
  AttachmentDesc attDesc[1] = {};

  attDesc[0].format = _depthStencilFormat;
  attDesc[0].samples = m_sampleCountFlags;
  attDesc[0].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
  attDesc[0].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
  attDesc[0].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
  attDesc[0].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
  attDesc[0].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
  attDesc[0].finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

  AttachmentRef dsRef = {
    0,
    VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL
  };

  SubpassDesc subPassDesc = {}; // memset
  subPassDesc.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
  subPassDesc.colorAttachmentCount = 0;
  subPassDesc.pColorAttachments = nullptr;
  subPassDesc.pDepthStencilAttachment = &dsRef;

  RenderPassInfo renderPassInfo = {}; // memset
  renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
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
    qWarning("Failed to create RenderPass: %d", result);
  }
}

/**
 *
 * @brief
 * @return RenderPass instance
 *
 * @note this method should only be available to PipelineHelper
 * as RenderPass instance should not be accessed independently.
 */
RenderPass RenderPassHelper::getRenderPass()
{
  if(!m_renderPass)
  {
    createCustomRenderPass();
  }

  return m_renderPass;
}
