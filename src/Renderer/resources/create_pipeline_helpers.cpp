/*****************************************************
 * Partial Class: Renderer
 * Members: Create Pipeline Helpers (Private)
 *****************************************************/

#include "Renderer.hpp"

using namespace sdfRay4d;
using namespace vk;

void Renderer::createPipelineCache()
{
  // Pipeline cache
  pipeline::CacheInfo pipelineCacheInfo;

  memset(&pipelineCacheInfo, 0, sizeof(pipelineCacheInfo));

  pipelineCacheInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;

  auto result = m_deviceFuncs->vkCreatePipelineCache(m_device, &pipelineCacheInfo, nullptr, &m_pipelineCache);

  if (result != VK_SUCCESS)
    qFatal("Failed to create pipeline cache: %d", result);
}

void Renderer::createPipelineLayout()
{
  // Pipeline layout
  pipeline::LayoutInfo pipelineLayoutInfo;

  memset(&pipelineLayoutInfo, 0, sizeof(pipelineLayoutInfo));

  pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
  pipelineLayoutInfo.setLayoutCount = 1;
  pipelineLayoutInfo.pSetLayouts = &m_descSetLayout;

  auto result = m_deviceFuncs->vkCreatePipelineLayout(m_device, &pipelineLayoutInfo, nullptr, &m_pipelineLayout);

  if (result != VK_SUCCESS)
    qFatal("Failed to create pipeline layout: %d", result);
}

void Renderer::createGraphicsPipeline()
{
  // Shaders
  ShaderModule vertexShader = createShader(
    QStringLiteral("assets/shaders/color.vert.spv"),
    VK_SHADER_STAGE_VERTEX_BIT
  );
  ShaderModule fragmentShader = createShader(
    QStringLiteral("assets/shaders/color.frag.spv"),
    VK_SHADER_STAGE_FRAGMENT_BIT
  );

  pipeline::ShaderStage shaderStages[2] = {
    {
      VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
      nullptr,
      0,
      VK_SHADER_STAGE_VERTEX_BIT,
      vertexShader,
      "main",
      nullptr
    },
    {
      VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
      nullptr,
      0,
      VK_SHADER_STAGE_FRAGMENT_BIT,
      fragmentShader,
      "main",
      nullptr
    }
  };






  VertexBindingDesc vertexBindingDesc = {
    0, // binding
    5 * sizeof(float),
    VK_VERTEX_INPUT_RATE_VERTEX
  };
  VertexAttrDesc vertexAttrDesc[] = {
    { // position
      0, // location
      0, // binding
      VK_FORMAT_R32G32_SFLOAT,
      0
    },
    { // color
      1,
      0,
      VK_FORMAT_R32G32B32_SFLOAT,
      2 * sizeof(float)
    }
  };

  pipeline::VertexInputState vertexInputInfo;
  vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
  vertexInputInfo.pNext = nullptr;
  vertexInputInfo.flags = 0;
  vertexInputInfo.vertexBindingDescriptionCount = 1;
  vertexInputInfo.pVertexBindingDescriptions = &vertexBindingDesc;
  vertexInputInfo.vertexAttributeDescriptionCount = 2;
  vertexInputInfo.pVertexAttributeDescriptions = vertexAttrDesc;




  // Graphics pipeline
  pipeline::GraphicsPipeline pipelineInfo;
  memset(&pipelineInfo, 0, sizeof(pipelineInfo));
  pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;

  pipelineInfo.stageCount = 2;
  pipelineInfo.pStages = shaderStages;//createShaderStages();
  pipelineInfo.pVertexInputState = &vertexInputInfo;//createVertexInputState();

  pipeline::InputAssembly ia;
  memset(&ia, 0, sizeof(ia));
  ia.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
  ia.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
  pipelineInfo.pInputAssemblyState = &ia;

  // The viewport and scissor will be set dynamically via vkCmdSetViewport/Scissor.
  // This way the pipeline does not need to be touched when resizing the window.
  pipeline::Viewport vp;
  memset(&vp, 0, sizeof(vp));
  vp.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
  vp.viewportCount = 1;
  vp.scissorCount = 1;
  pipelineInfo.pViewportState = &vp;

  pipeline::Rasterization rs;
  memset(&rs, 0, sizeof(rs));
  rs.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
  rs.polygonMode = VK_POLYGON_MODE_FILL;
  rs.cullMode = VK_CULL_MODE_NONE; // we want the back face as well
  rs.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
  rs.lineWidth = 1.0f;
  pipelineInfo.pRasterizationState = &rs;

  pipeline::Multisample ms;
  memset(&ms, 0, sizeof(ms));
  ms.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
  // Enable multisampling.
  ms.rasterizationSamples = m_vkWindow->sampleCountFlagBits();
  pipelineInfo.pMultisampleState = &ms;

  pipeline::DepthStencil ds;
  memset(&ds, 0, sizeof(ds));
  ds.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
  ds.depthTestEnable = VK_TRUE;
  ds.depthWriteEnable = VK_TRUE;
  ds.depthCompareOp = VK_COMPARE_OP_LESS_OR_EQUAL;
  pipelineInfo.pDepthStencilState = &ds;

  pipeline::ColorBlend cb;
  memset(&cb, 0, sizeof(cb));
  cb.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
  // no blend, write out all of rgba
  pipeline::ColorBlendAttachment att;
  memset(&att, 0, sizeof(att));
  att.colorWriteMask = 0xF;
  cb.attachmentCount = 1;
  cb.pAttachments = &att;
  pipelineInfo.pColorBlendState = &cb;

  DynamicState dynEnable[] = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };
  pipeline::Dynamic dyn;
  memset(&dyn, 0, sizeof(dyn));
  dyn.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
  dyn.dynamicStateCount = sizeof(dynEnable) / sizeof(VkDynamicState);
  dyn.pDynamicStates = dynEnable;
  pipelineInfo.pDynamicState = &dyn;

  pipelineInfo.layout = m_pipelineLayout;
  pipelineInfo.renderPass = m_vkWindow->defaultRenderPass();

  auto result = m_deviceFuncs->vkCreateGraphicsPipelines(m_device, m_pipelineCache, 1, &pipelineInfo, nullptr, &m_pipeline);
  if (result != VK_SUCCESS)
    qFatal("Failed to create graphics pipeline: %d", result);

  if (vertexShader)   m_deviceFuncs->vkDestroyShaderModule(m_device, vertexShader, nullptr);
  if (fragmentShader) m_deviceFuncs->vkDestroyShaderModule(m_device, fragmentShader, nullptr);
}

//VkPipelineShaderStageCreateInfo *Renderer::createShaderStages()
//{
//  VkPipelineShaderStageCreateInfo shaderStages[2] = {
//    {
//      VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
//      nullptr,
//      0,
//      VK_SHADER_STAGE_VERTEX_BIT,
//      vertShaderModule,
//      "main",
//      nullptr
//    },
//    {
//      VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
//      nullptr,
//      0,
//      VK_SHADER_STAGE_FRAGMENT_BIT,
//      fragShaderModule,
//      "main",
//      nullptr
//    }
//  };
//
//  return shaderStages;
//}

//VkPipelineVertexInputStateCreateInfo Renderer::createVertexInputState()
//{
//  VkVertexInputBindingDescription vertexBindingDesc = {
//    0, // binding
//    5 * sizeof(float),
//    VK_VERTEX_INPUT_RATE_VERTEX
//  };
//  VkVertexInputAttributeDescription vertexAttrDesc[] = {
//    { // position
//      0, // location
//      0, // binding
//      VK_FORMAT_R32G32_SFLOAT,
//      0
//    },
//    { // color
//      1,
//      0,
//      VK_FORMAT_R32G32B32_SFLOAT,
//      2 * sizeof(float)
//    }
//  };
//
//  VkPipelineVertexInputStateCreateInfo vertexInputInfo;
//  vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
//  vertexInputInfo.pNext = nullptr;
//  vertexInputInfo.flags = 0;
//  vertexInputInfo.vertexBindingDescriptionCount = 1;
//  vertexInputInfo.pVertexBindingDescriptions = &vertexBindingDesc;
//  vertexInputInfo.vertexAttributeDescriptionCount = 2;
//  vertexInputInfo.pVertexAttributeDescriptions = vertexAttrDesc;
//
//  return vertexInputInfo;
//}
