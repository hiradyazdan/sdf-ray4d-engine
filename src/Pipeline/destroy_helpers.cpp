/*****************************************************
 * Partial Class: Pipeline
 * Members: Destroy Helpers (Public)
 *****************************************************/

#include "Pipeline.hpp"

using namespace sdfRay4d;

void PipelineHelper::destroyPipeline(
  const MaterialPtr &_material
)
{
  if (_material->pipeline)
  {
    m_deviceFuncs->vkDestroyPipeline(
      m_device,
      _material->pipeline,
      nullptr
    );
    _material->pipeline = VK_NULL_HANDLE;
  }
}

void PipelineHelper::destroyPipelineLayout(
  const MaterialPtr &_material
)
{
  if (_material->pipelineLayout)
  {
    m_deviceFuncs->vkDestroyPipelineLayout(
      m_device,
      _material->pipelineLayout,
      nullptr
    );
    _material->pipelineLayout = VK_NULL_HANDLE;
  }
}

void PipelineHelper::destroyPipelines()
{
  for(const auto &material : m_materials)
  {
    destroyPipeline(material);
    destroyPipelineLayout(material);
  }

  if (m_pipelineCache)
  {
    m_deviceFuncs->vkDestroyPipelineCache(
      m_device,
      m_pipelineCache,
      nullptr
    );
    m_pipelineCache = VK_NULL_HANDLE;
  }
}

void PipelineHelper::destroyDescriptors()
{
  for(const auto &material : m_materials)
  {
    if (material->descSetLayout)
    {
      m_deviceFuncs->vkDestroyDescriptorSetLayout(
        m_device,
        material->descSetLayout,
        nullptr
      );
      material->descSetLayout = VK_NULL_HANDLE;
    }

    // TODO: Is this needed?
    if (material->descSetLayouts)
    {
      material->descSetLayouts = VK_NULL_HANDLE;
    }

    if (material->descPool)
    {
      m_deviceFuncs->vkDestroyDescriptorPool(
        m_device,
        material->descPool,
        nullptr
      );
      material->descPool = VK_NULL_HANDLE;
    }
  }
}
