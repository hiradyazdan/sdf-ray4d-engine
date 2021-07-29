/*****************************************************
 * Partial Class: Renderer
 * Members: Create Pipeline Helpers (Private)
 *****************************************************/

#include "Renderer.hpp"

using namespace sdfRay4d;

void Renderer::createPipelines()
{
  createPipelineCache();
  createObjPipeline();
}

void Renderer::createPipelineCache()
{
  /**
   * TODO: Why should I need this?
   */

  // Pipeline cache
  pipeline::CacheInfo pipelineCacheInfo;

  memset(&pipelineCacheInfo, 0, sizeof(pipelineCacheInfo));

  pipelineCacheInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;

  auto result = m_deviceFuncs->vkCreatePipelineCache(
    m_device,
    &pipelineCacheInfo,
    nullptr,
    &m_pipelineCache
  );

  if (result != VK_SUCCESS)
  {
    qFatal("Failed to create pipeline cache: %d", result);
  }
}

void Renderer::createObjPipeline()
{
  createPipelineLayout();
  createGraphicsPipeline();
}

void Renderer::createPipelineLayout()
{
  // Pipeline layout
  pipeline::LayoutInfo pipelineLayoutInfo;
  PushConstantRange pushConstant;
  pushConstant.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
  pushConstant.offset = 0;
  pushConstant.size = 64;

  memset(&pipelineLayoutInfo, 0, sizeof(pipelineLayoutInfo));

  pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
  pipelineLayoutInfo.pushConstantRangeCount = 1;
  pipelineLayoutInfo.pPushConstantRanges = &pushConstant;
//  pipelineLayoutInfo.setLayoutCount = 1;
//  pipelineLayoutInfo.pSetLayouts = &m_descSetLayout;

  auto result = m_deviceFuncs->vkCreatePipelineLayout(
    m_device,
    &pipelineLayoutInfo,
    nullptr,
    &m_objMaterial.pipelineLayout
  );

  if (result != VK_SUCCESS)
  {
    qFatal("Failed to create pipeline layout: %d", result);
  }
}

void Renderer::createGraphicsPipeline()
{
  pipeline::InputAssemblyInfo inputAssembly;
  memset(&inputAssembly, 0, sizeof(inputAssembly));
  inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
  inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
  inputAssembly.primitiveRestartEnable = VK_FALSE;

  pipeline::RasterizationInfo rasterization;
  memset(&rasterization, 0, sizeof(rasterization));
  rasterization.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
  rasterization.depthClampEnable = VK_FALSE;
  rasterization.rasterizerDiscardEnable = VK_FALSE;
  rasterization.polygonMode = VK_POLYGON_MODE_FILL;
  rasterization.cullMode = VK_CULL_MODE_BACK_BIT;
  rasterization.frontFace = VK_FRONT_FACE_CLOCKWISE;
  rasterization.depthBiasEnable = VK_FALSE;
  rasterization.depthBiasConstantFactor = 0.0f;
  rasterization.depthBiasClamp = 0.0f;
  rasterization.depthBiasSlopeFactor = 0.0f;
  rasterization.lineWidth = 1.0f;

  pipeline::ViewportInfo viewportState;
  memset(&viewportState, 0, sizeof(viewportState));
  viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
  viewportState.viewportCount = 1;
  viewportState.pViewports = nullptr;
  viewportState.scissorCount = 1;
  viewportState.pScissors = nullptr;

  pipeline::MultisampleInfo multisample;
  memset(&multisample, 0, sizeof(multisample));
  multisample.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
  multisample.rasterizationSamples = m_isMSAA ? m_vkWindow->sampleCountFlagBits() : VK_SAMPLE_COUNT_1_BIT;
  multisample.sampleShadingEnable = VK_FALSE;
  multisample.minSampleShading = 1.0f;
  multisample.pSampleMask = nullptr;
  multisample.alphaToCoverageEnable = VK_FALSE;
  multisample.alphaToOneEnable = VK_FALSE;

  pipeline::DepthStencilInfo depthStencil;
  memset(&depthStencil, 0, sizeof(depthStencil));
  depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
  depthStencil.depthTestEnable = VK_TRUE;
  depthStencil.depthWriteEnable = VK_TRUE;
  depthStencil.depthCompareOp = VK_COMPARE_OP_LESS_OR_EQUAL;

  pipeline::ColorBlendInfo colorBlend;
  memset(&colorBlend, 0, sizeof(colorBlend));
  colorBlend.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
  colorBlend.logicOpEnable = VK_FALSE;
  colorBlend.attachmentCount = 1;
  // no blend, write out all of rgba
  pipeline::ColorBlendAttachment att;
  memset(&att, 0, sizeof(att));

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

  colorBlend.pAttachments = &att;

  pipeline::DynamicInfo dynamicInfo;
  memset(&dynamicInfo, 0, sizeof(dynamicInfo));

  DynamicState dynamicState[] = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };

  dynamicInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
  dynamicInfo.dynamicStateCount = sizeof(dynamicState) / sizeof(DynamicState); // 2
  dynamicInfo.pDynamicStates = dynamicState;

  pipeline::ShaderStageInfo shaderStages[2] = {
    {
      VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO, // sType
      nullptr, // pNext
      0, // flags
      VK_SHADER_STAGE_VERTEX_BIT, // stage
      m_objMaterial.vertexShader.getData()->shaderModule, // module
      "main", // pName
      nullptr // pSpecializationInfo
    },
    {
      VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
      nullptr,
      0,
      VK_SHADER_STAGE_FRAGMENT_BIT,
      m_objMaterial.fragmentShader.getData()->shaderModule,
      "main",
      nullptr
    }
  };

  VertexBindingDesc vertexBindingDesc[] = {
    {
      0, // binding
      3 * sizeof(float),
      VK_VERTEX_INPUT_RATE_VERTEX
    }
  };
  VertexAttrDesc vertexAttrDesc[] = {
    { // position
      0, // location
      0, // binding
      VK_FORMAT_R32G32B32_SFLOAT,
      0
    },
    { // normal
      1,
      0,
      VK_FORMAT_R32G32B32_SFLOAT, // VK_FORMAT_R8G8B8A8_UNORM
      5 * sizeof(float)
    }
  };

  pipeline::VertexInputInfo vertexInputInfo;
  vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
  vertexInputInfo.vertexBindingDescriptionCount = sizeof(vertexBindingDesc) / sizeof(vertexBindingDesc[0]);
  vertexInputInfo.pVertexBindingDescriptions = vertexBindingDesc;
  vertexInputInfo.vertexAttributeDescriptionCount = sizeof(vertexAttrDesc) / sizeof(vertexAttrDesc[0]);
  vertexInputInfo.pVertexAttributeDescriptions = vertexAttrDesc;
  vertexInputInfo.pNext = nullptr;
  vertexInputInfo.flags = 0;

  pipeline::GraphicsPipelineInfo pipelineInfo;
  memset(&pipelineInfo, 0, sizeof(pipelineInfo));
  pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
  pipelineInfo.stageCount = 2;
  pipelineInfo.pStages = shaderStages;
  pipelineInfo.pVertexInputState = &vertexInputInfo;
  pipelineInfo.pInputAssemblyState = &inputAssembly;
  pipelineInfo.pViewportState = &viewportState;
  pipelineInfo.pRasterizationState = &rasterization;
  pipelineInfo.pMultisampleState = &multisample;
  pipelineInfo.pDepthStencilState = &depthStencil;
  pipelineInfo.pColorBlendState = &colorBlend;
  pipelineInfo.pDynamicState = &dynamicInfo;
  pipelineInfo.layout = m_objMaterial.pipelineLayout;
  pipelineInfo.renderPass = m_vkWindow->defaultRenderPass();

  auto result = m_deviceFuncs->vkCreateGraphicsPipelines(
    m_device,
    m_pipelineCache,
    1,
    &pipelineInfo,
    nullptr,
    &m_objMaterial.pipeline
  );

  if (result != VK_SUCCESS)
  {
    qFatal("Failed to create graphics pipeline: %d", result);
  }
}
