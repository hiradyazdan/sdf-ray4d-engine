/*****************************************************
 * Partial Class: Renderer
 * Members: Create Pipeline Helpers (Private)
 *****************************************************/

#include "Renderer.hpp"

using namespace sdfRay4d;

void Renderer::createPipelineCache()
{
  /**
   * NOTE:
   *
   * This will reduce pipeline (re)creation cost
   *
   * Just in case to cache the layouts/descriptor sets & shader module
   * though most modern drivers have a cache of their own
   */
  pipeline::CacheInfo pipelineCacheInfo = {}; // memset

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

void Renderer::createSDFPipeline()
{
  createPipelineWorker(
    m_sdfPipelineWorker,
    m_sdfMaterial
  );
}

void Renderer::createPipelineWorker(
  QFuture<void> &_pipelineFuture,
  const MaterialPtr &_material
)
{
  _pipelineFuture = QtConcurrent::run(
    this, &Renderer::createMaterialPipeline,
    _material
  );
}

/**
 * per Material Pipeline
 */
void Renderer::createMaterialPipeline(const MaterialPtr &_material)
{
  createPipelineLayout(_material);
  createGraphicsPipeline(_material);

  /**
   * TODO: Should I create buffers at this stage?
   * if buffers don't need to update per frame
   */
//  createBuffers();
}

/**
 * per Material Pipeline
 */
void Renderer::createPipelineLayout(const MaterialPtr &_material)
{
  PushConstantRange pushConstant;
  pushConstant.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
  pushConstant.offset = 0;
  pushConstant.size = 64;

  pipeline::LayoutInfo pipelineLayoutInfo = {}; // memset
  pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
  pipelineLayoutInfo.pushConstantRangeCount = 1;
  pipelineLayoutInfo.pPushConstantRanges = &pushConstant;
//  pipelineLayoutInfo.setLayoutCount = 1;
//  pipelineLayoutInfo.pSetLayouts = &m_descSetLayout;

  auto result = m_deviceFuncs->vkCreatePipelineLayout(
    m_device,
    &pipelineLayoutInfo,
    nullptr,
    &_material->pipelineLayout
  );

  if (result != VK_SUCCESS)
  {
    qFatal("Failed to create pipeline layout: %d", result);
  }
}

/**
 * TODO: implement compute pipeline with compute shaders
 *
 * per Material Pipeline
 */
void Renderer::createComputePipeline(const MaterialPtr &_material)
{
//  initShaderStages();
//  initPSOs();

  auto &pso = _material->pso;

  pipeline::ComputePipelineInfo pipelineInfo = {};

  pipelineInfo.sType                = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
//  pipelineInfo.stage                = m_objMaterial->shaderStages.data();
  pipelineInfo.layout               = _material->pipelineLayout;

  auto result = m_deviceFuncs->vkCreateComputePipelines(
    m_device,
    m_pipelineCache,
    1,
    &pipelineInfo,
    nullptr,
    &_material->pipeline
  );

  if (result != VK_SUCCESS)
  {
    qFatal("Failed to create compute pipeline: %d", result);
  }
}

/**
 * per Material Pipeline
 */
void Renderer::createGraphicsPipeline(const MaterialPtr &_material)
{
  initShaderStages(_material);
  initPSOs(_material);

  auto &pso = _material->pso;

  pipeline::GraphicsPipelineInfo pipelineInfo = {}; // memset
  pipelineInfo.sType                = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
  pipelineInfo.stageCount           = 2;
  pipelineInfo.pStages              = _material->shaderStages.data();

  // Set Pipeline State Objects (PSOs)
  pipelineInfo.pDynamicState        = &pso.dynamicState;
  pipelineInfo.pVertexInputState    = &pso.vertexInputState;
  pipelineInfo.pInputAssemblyState  = &pso.inputAssemblyState;
  pipelineInfo.pRasterizationState  = &pso.rasterizationState;
  pipelineInfo.pColorBlendState     = &pso.colorBlendState;
  pipelineInfo.pViewportState       = &pso.viewportState;
  pipelineInfo.pDepthStencilState   = &pso.depthStencilState;
  pipelineInfo.pMultisampleState    = &pso.multisampleState;

  pipelineInfo.layout               = _material->pipelineLayout;
  pipelineInfo.renderPass           = m_vkWindow->defaultRenderPass();

  auto result = m_deviceFuncs->vkCreateGraphicsPipelines(
    m_device,
    m_pipelineCache,
    1,
    &pipelineInfo,
    nullptr,
    &_material->pipeline
  );

  if (result != VK_SUCCESS)
  {
    qFatal("Failed to create graphics pipeline: %d", result);
  }

  /*
   * NOTE:
   *
   * This may be a micro optimization:
   *
   * Just in case if memory matters at all here, though seems unlikely
   * as the usage of the heap memory on PSOs for the renderer is very minimal
   * and CPU performance is not a big concern here
   */
  _material->pso = {}; // memset
}

/**
 * per Material Pipeline
 */
void Renderer::initShaderStages(const MaterialPtr &_material)
{
  _material->shaderStages = {
    {
      VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO, // sType
      nullptr, // pNext
      0, // flags
      VK_SHADER_STAGE_VERTEX_BIT, // stage
      _material->vertexShader.getData()->shaderModule, // module
      "main", // pName
      nullptr // pSpecializationInfo
    },
    {
      VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO, // sType
      nullptr, // pNext
      0, // flags
      VK_SHADER_STAGE_FRAGMENT_BIT, // stage
      _material->fragmentShader.getData()->shaderModule, // module
      "main", // pName
      nullptr // pSpecializationInfo
    }
  };
}

/**
 * per Material Pipeline
 */
void Renderer::setupDescriptorSets(const MaterialPtr &_material)
{

  /**
   * TODO: Rewrite this
   */

  // Descriptor set layout.
  VkDescriptorPoolSize descPoolSizes[] = {
    { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 2 }
  };
  VkDescriptorPoolCreateInfo descPoolInfo;
  memset(&descPoolInfo, 0, sizeof(descPoolInfo));
  descPoolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
  descPoolInfo.maxSets = 1; // a single set is enough due to the dynamic uniform buffer
  descPoolInfo.poolSizeCount = sizeof(descPoolSizes) / sizeof(descPoolSizes[0]);
  descPoolInfo.pPoolSizes = descPoolSizes;
  VkResult err = m_deviceFuncs->vkCreateDescriptorPool(
    m_device,
    &descPoolInfo,
    nullptr,
    &_material->descPool
  );

  if (err != VK_SUCCESS)
  {
    qFatal("Failed to create descriptor pool: %d", err);
  }

  VkDescriptorSetLayoutBinding layoutBindings[] =
    {
      {
        0, // binding
        VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC,
        1, // descriptorCount
        VK_SHADER_STAGE_VERTEX_BIT,
        nullptr
      },
      {
        1,
        VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC,
        1,
        VK_SHADER_STAGE_FRAGMENT_BIT,
        nullptr
      }
    };
  VkDescriptorSetLayoutCreateInfo descLayoutInfo = {
    VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
    nullptr,
    0,
    sizeof(layoutBindings) / sizeof(layoutBindings[0]),
    layoutBindings
  };
  err = m_deviceFuncs->vkCreateDescriptorSetLayout(
    m_device,
    &descLayoutInfo,
    nullptr,
    &_material->descSetLayout
  );
  if (err != VK_SUCCESS)
    qFatal("Failed to create descriptor set layout: %d", err);

  VkDescriptorSetAllocateInfo descSetAllocInfo = {
    VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
    nullptr,
    _material->descPool,
    1,
    &_material->descSetLayout
  };
  err = m_deviceFuncs->vkAllocateDescriptorSets(m_device, &descSetAllocInfo, &_material->descSet);
  if (err != VK_SUCCESS)
    qFatal("Failed to allocate descriptor set: %d", err);
}
