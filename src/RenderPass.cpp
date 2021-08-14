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
  const RenderPass &_renderPass,
  bool _useDefault
) :
  m_device(_device)
  , m_deviceFuncs(_deviceFuncs)
  , m_sampleCountFlags(_sampleCountFlags)
{
  if(!_useDefault)
  {
    createCustomRenderPass();
  }
}

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
 * @param _colorFormat
 * @param _depthStencilFormat
 */
void RenderPassHelper::createCustomRenderPass(
  const Format &_colorFormat,
  const Format &_depthStencilFormat
)
{
  const bool isMsaa = m_sampleCountFlags > VK_SAMPLE_COUNT_1_BIT;

  AttachmentDesc attDesc[3] = {};

  // This is either the non-msaa render target or the resolve target.
  attDesc[0].format = _colorFormat;
  attDesc[0].samples = VK_SAMPLE_COUNT_1_BIT;
  attDesc[0].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR; // ignored when msaa
  attDesc[0].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
  attDesc[0].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
  attDesc[0].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
  attDesc[0].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
  attDesc[0].finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

  attDesc[1].format = _depthStencilFormat;
  attDesc[1].samples = m_sampleCountFlags;
  attDesc[1].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
  attDesc[1].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
  attDesc[1].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
  attDesc[1].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
  attDesc[1].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
  attDesc[1].finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

  if (isMsaa) {
    // msaa render target
    attDesc[2].format = _colorFormat;
    attDesc[2].samples = m_sampleCountFlags;
    attDesc[2].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    attDesc[2].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    attDesc[2].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    attDesc[2].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    attDesc[2].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    attDesc[2].finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
  }

  AttachmentRef colorRef = { 0, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL };
  AttachmentRef resolveRef = { 0, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL };
  AttachmentRef dsRef = { 1, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL };

  SubpassDesc subPassDesc = {}; // memset
  subPassDesc.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
  subPassDesc.colorAttachmentCount = 1;
  subPassDesc.pColorAttachments = &colorRef;
  subPassDesc.pDepthStencilAttachment = &dsRef;

  RenderPassInfo renderPassInfo = {}; // memset
  renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
  renderPassInfo.attachmentCount = 2;
  renderPassInfo.pAttachments = attDesc;
  renderPassInfo.subpassCount = 1;
  renderPassInfo.pSubpasses = &subPassDesc;

  if (isMsaa)
  {
    colorRef.attachment = 2;
    subPassDesc.pResolveAttachments = &resolveRef;
    renderPassInfo.attachmentCount = 3;
  }

  auto result = m_deviceFuncs->vkCreateRenderPass(
    m_device,
    &renderPassInfo,
    nullptr,
    &m_customRenderPass
  );

  if (result != VK_SUCCESS) {
    qWarning("Failed to create RenderPass: %d", result);
  }
}
