/*****************************************************
 * Partial Class: PipelineHelper
 * Members: Create Pipeline Helpers (Public/Private)
 *****************************************************/

#include "Helpers/Pipeline.hpp"

using namespace sdfRay4d::helpers;

void PipelineHelper::createCache() noexcept
{
  /**
   * @note
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

void PipelineHelper::createPipelines() noexcept
{
  for(const auto &material : m_materials)
  {
    createPipeline(material);
  }
}

/**
 * @param[in] _material
 */
void PipelineHelper::createPipeline(
  const MaterialPtr &_material
) noexcept
{
  /**
   * @note makes pipeline creation thread safe
   * if each pipeline runs on a separate
   * async CPU thread
   */
  QMutexLocker locker(&m_pipeMutex);

  /**
   * @note SDFR Pipeline is always marked as swappable
   */
  if(_material->isHotSwappable)
  {
    const auto &shaderData = _material->vertexShader.getData();

    if(!m_isHot) // initial load
    {
      /**
       * @note stores initially loaded shader module
       * for later destruction after swapping with
       * new pipelines
       */
      m_currentShaderModule = shaderData->shaderModule;
    }
    else // SDF Graph compile
    {
      /**
       * @note restores initially loaded shader module
       * when swapping old with new pipeline, once SDF Graph
       * compiled and created new pipeline.
       */
      shaderData->shaderModule = m_currentShaderModule;
    }
  }

  descriptor.createDescriptorSets(_material);
  createLayout(_material);
  createGraphicsPipeline(_material);

  /**
   * TODO: Should I create buffers at this stage?
   * if buffers don't need to update per frame (static)
   */
//  createBuffers();
}

/**
 *
 * @param[in] _oldMaterial
 * @param[in] _newMaterial
 */
void PipelineHelper::swapSDFRPipelines(
  const MaterialPtr &_oldMaterial,
  const MaterialPtr &_newMaterial
) noexcept
{
  // safe-guard in case if this is invoked elsewhere
  if(!m_isHot) return;

  m_isHot = false;

  /**
   * @note When newly created pipeline is ready
   * swap the old with the new one
   */
  _oldMaterial->pipelineLayout  = _newMaterial->pipelineLayout;
  _oldMaterial->pipeline        = _newMaterial->pipeline;
}

/**
 *
 * @param[in] _material
 */
void PipelineHelper::createLayout(
  const MaterialPtr &_material
) noexcept
{
  pipeline::LayoutInfo pipelineLayoutInfo = {}; // memset
  pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
  pipelineLayoutInfo.pushConstantRangeCount = _material->pushConstantRangeCount;
  pipelineLayoutInfo.pPushConstantRanges = &_material->pushConstantRange;
  pipelineLayoutInfo.setLayoutCount = _material->descSetLayoutCount;
  pipelineLayoutInfo.pSetLayouts = _material->descSetLayouts.data();

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
void PipelineHelper::createComputePipeline(
  const MaterialPtr &_material
) noexcept
{
  //  initShaderStages();
  //  initPSOs();

  auto &pso = _material->pso;

  pipeline::ComputePipelineInfo pipelineInfo = {}; // memset

  pipelineInfo.sType                = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
  //  pipelineInfo.stage                = m_actorMaterial->shaderStages.data();
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
 * @param[in] _material
 */
void PipelineHelper::createGraphicsPipeline(
  const MaterialPtr &_material
) noexcept
{
  initShaderStages(_material);
  initPSOs(_material);

  auto &&shaderStages = _material->shaderStages;
  auto &&pso = _material->pso;

  pipeline::GraphicsPipelineInfo pipelineInfo = {}; // memset
  pipelineInfo.sType                = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
  pipelineInfo.stageCount           = shaderStages.size();
  pipelineInfo.pStages              = shaderStages.data();

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
  pipelineInfo.renderPass           = _material->renderPass;

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
   * @note
   *
   * This may be a micro optimization:
   *
   * Just in case if memory matters at all here, though seems unlikely
   * as the usage of the heap memory on PSOs for the renderer is very minimal
   * and CPU performance is not a big concern here
   */
  pso = {}; // memset
}

/**
 * per Material Pipeline
 * @param[in] _material
 */
void PipelineHelper::initShaderStages(
  const MaterialPtr &_material
) noexcept
{
  const auto &structureType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;

  _material->shaderStages = {
    {
      structureType, // sType
      nullptr, // pNext
      0, // flags
      VK_SHADER_STAGE_VERTEX_BIT, // stage
      _material->vertexShader.getData()->shaderModule, // module
      "main", // pName
      nullptr // pSpecializationInfo
    },
    {
      structureType, // sType
      nullptr, // pNext
      0, // flags
      VK_SHADER_STAGE_FRAGMENT_BIT, // stage
      _material->fragmentShader.getData()->shaderModule, // module
      "main", // pName
      nullptr // pSpecializationInfo
    }
  };
}
