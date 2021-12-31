/*****************************************************
 * Partial Class: PipelineHelper
 * Members: Init PSO (Pipeline State Objects) Helpers (Private)
 *****************************************************/

#include "VKHelpers/Pipeline.hpp"

using namespace sdfRay4d::vkHelpers;

/**
 *
 * @param[in] _material
 */
void PipelineHelper::initPSOs(
  const MaterialPtr &_material
) noexcept
{
  setDynamicState(_material);
  setVertexInputState(_material);
  setInputAssemblyState(_material);
  setRasterizationState(_material);
  setColorBlendState(_material);
  setViewportState(_material);
  setDepthStencilState(_material);
  setMultisampleState(_material);
}

/**
 *
 * @param[in] _material
 */
void PipelineHelper::setDynamicState(
  const MaterialPtr &_material
) noexcept
{
  auto &pso = _material->pso;
  auto &dynamicState = pso.dynamicState = {}; // memset
  auto &states = pso.dynamicStates = {
    VK_DYNAMIC_STATE_VIEWPORT,
    VK_DYNAMIC_STATE_SCISSOR
  };

  dynamicState.sType = pipeline::StructureType::DYNAMIC_INFO;
  dynamicState.dynamicStateCount = states.size(); // 2
  dynamicState.pDynamicStates = states.data();
}

/**
 *
 * @param[in] _material
 */
void PipelineHelper::setVertexInputState(
  const MaterialPtr &_material
) noexcept
{
  auto &pso = _material->pso;
  auto &vertexInputState = pso.vertexInputState = {}; // memset
  auto &vertexBindingDesc = pso.vertexBindingDesc = {
    {
      0, // binding
      3 * sizeof(float), // stride
      VK_VERTEX_INPUT_RATE_VERTEX // inputRate
    }
  };
  auto &vertexAttrDesc = pso.vertexAttrDesc = {
    { // position
      0, // location
      0, // binding
      VK_FORMAT_R32G32B32_SFLOAT, // format
      0 // offset
    },
    { // normal
      1, // location
      0, // binding
      VK_FORMAT_R32G32B32_SFLOAT, // format // VK_FORMAT_R8G8B8A8_UNORM
      3 * sizeof(float) // offset (vec3)
    },
    { // texCoord
      2, // location
      0, // binding
      VK_FORMAT_R32G32B32_SFLOAT, // format
      2 * sizeof(float) // offset (vec2)
    }
  };

  vertexInputState.sType = pipeline::StructureType::VERTEX_INPUT_INFO;
  vertexInputState.vertexBindingDescriptionCount = vertexBindingDesc.size();
  vertexInputState.pVertexBindingDescriptions = vertexBindingDesc.data();
  vertexInputState.vertexAttributeDescriptionCount = vertexAttrDesc.size();
  vertexInputState.pVertexAttributeDescriptions = vertexAttrDesc.data();
  vertexInputState.pNext = nullptr;
  vertexInputState.flags = 0;
}

/**
 *
 * @param[in] _material
 */
void PipelineHelper::setInputAssemblyState(
  const MaterialPtr &_material
) noexcept
{
  auto &pso = _material->pso;
  auto &inputAssemblyState = pso.inputAssemblyState = {}; // memset

  inputAssemblyState.sType = pipeline::StructureType::INPUT_ASSEMBLY_INFO;
  inputAssemblyState.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
  inputAssemblyState.primitiveRestartEnable = VK_FALSE;
}

/**
 *
 * @param[in] _material
 */
void PipelineHelper::setRasterizationState(
  const MaterialPtr &_material
) noexcept
{
  auto &pso = _material->pso;
  auto &rasterizationState = pso.rasterizationState = {}; // memset

  rasterizationState.sType = pipeline::StructureType::RASTERIZATION_INFO;
  rasterizationState.depthClampEnable = VK_FALSE;
  rasterizationState.rasterizerDiscardEnable = VK_FALSE;
  rasterizationState.polygonMode = VK_POLYGON_MODE_FILL;
  rasterizationState.cullMode = VK_CULL_MODE_BACK_BIT;
  rasterizationState.frontFace = VK_FRONT_FACE_CLOCKWISE;
  rasterizationState.depthBiasEnable = VK_FALSE;
  rasterizationState.depthBiasConstantFactor = 0.0f;
  rasterizationState.depthBiasClamp = 0.0f;
  rasterizationState.depthBiasSlopeFactor = 0.0f;
  rasterizationState.lineWidth = 1.0f;
}

/**
 *
 * @param[in] _material
 */
void PipelineHelper::setColorBlendState(
  const MaterialPtr &_material
) noexcept
{
  auto &pso = _material->pso;
  auto &colorBlendState = pso.colorBlendState = {}; // memset

  colorBlendState.sType = pipeline::StructureType::COLOR_BLEND_INFO;
  colorBlendState.logicOpEnable = VK_FALSE;
  colorBlendState.attachmentCount = 1;

  // no blend, write out all of rgba
  auto &att = pso.colorBlendAttachment = {}; // memset

  att.blendEnable = VK_FALSE;
  att.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
  att.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
  att.colorBlendOp = VK_BLEND_OP_ADD;
  att.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
  att.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
  att.alphaBlendOp = VK_BLEND_OP_ADD;
  att.colorWriteMask =
    VK_COLOR_COMPONENT_R_BIT |
    VK_COLOR_COMPONENT_G_BIT |
    VK_COLOR_COMPONENT_B_BIT |
    VK_COLOR_COMPONENT_A_BIT;

  colorBlendState.pAttachments = &att;
}

/**
 *
 * @param[in] _material
 */
void PipelineHelper::setViewportState(
  const MaterialPtr &_material
) noexcept
{
  auto &pso = _material->pso;
  auto &viewportState = pso.viewportState = {}; // memset

  viewportState.sType = pipeline::StructureType::VIEWPORT_INFO;
  viewportState.viewportCount = 1;
  viewportState.pViewports = nullptr;
  viewportState.scissorCount = 1;
  viewportState.pScissors = nullptr;
}

/**
 *
 * @param[in] _material
 */
void PipelineHelper::setDepthStencilState(
  const MaterialPtr &_material
) noexcept
{
  auto &pso = _material->pso;
  auto &depthStencilState = pso.depthStencilState = {}; // memset

  depthStencilState.sType = pipeline::StructureType::DEPTH_STENCIL_INFO;
  depthStencilState.depthTestEnable = VK_TRUE;
  depthStencilState.depthWriteEnable = VK_TRUE;
  depthStencilState.depthCompareOp = VK_COMPARE_OP_LESS_OR_EQUAL;
}

/**
 *
 * @param[in] _material
 */
void PipelineHelper::setMultisampleState(
  const MaterialPtr &_material
) noexcept
{
  auto &pso = _material->pso;
  auto &multisampleState = pso.multisampleState = {}; // memset

  multisampleState.sType = pipeline::StructureType::MULTISAMPLE_INFO;
  multisampleState.rasterizationSamples = m_sampleCountFlags;
  multisampleState.sampleShadingEnable = VK_FALSE;
  multisampleState.minSampleShading = 1.0f;
  multisampleState.pSampleMask = nullptr;
  multisampleState.alphaToCoverageEnable = VK_FALSE;
  multisampleState.alphaToOneEnable = VK_FALSE;
}
