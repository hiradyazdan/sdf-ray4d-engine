/*****************************************************
 * Partial Class: Renderer
 * Members: Destroy Resources helpers (Private)
 *****************************************************/

#include "Renderer.hpp"

using namespace sdfRay4d;

void Renderer::destroyDescriptorSets()
{
  if (m_objMaterial.descSetLayout)
  {
    m_deviceFuncs->vkDestroyDescriptorSetLayout(m_device, m_objMaterial.descSetLayout, nullptr);
    m_objMaterial.descSetLayout = VK_NULL_HANDLE;
  }

  if (m_objMaterial.descPool)
  {
    m_deviceFuncs->vkDestroyDescriptorPool(m_device, m_objMaterial.descPool, nullptr);
    m_objMaterial.descPool = VK_NULL_HANDLE;
  }
}

void Renderer::destroyPipeline()
{
  if (m_objMaterial.pipeline)
  {
    m_deviceFuncs->vkDestroyPipeline(m_device, m_objMaterial.pipeline, nullptr);
    m_objMaterial.pipeline = VK_NULL_HANDLE;
  }

  if (m_objMaterial.pipelineLayout)
  {
    m_deviceFuncs->vkDestroyPipelineLayout(m_device, m_objMaterial.pipelineLayout, nullptr);
    m_objMaterial.pipelineLayout = VK_NULL_HANDLE;
  }

  if (m_pipelineCache)
  {
    m_deviceFuncs->vkDestroyPipelineCache(m_device, m_pipelineCache, nullptr);
    m_pipelineCache = VK_NULL_HANDLE;
  }
}

void Renderer::destroyBuffers()
{
//  vmaDestroyBuffer(m_allocator, buffer, allocation);
//  vmaDestroyAllocator(m_allocator);

  if (m_uniformBuffer)
  {
    m_deviceFuncs->vkDestroyBuffer(m_device, m_uniformBuffer, nullptr);
    m_uniformBuffer = VK_NULL_HANDLE;
  }

  if (m_bufMem)
  {
    m_deviceFuncs->vkFreeMemory(m_device, m_bufMem, nullptr);
    m_bufMem = VK_NULL_HANDLE;
  }
}

void Renderer::destroyShaderModules()
{
  auto vertexShader = m_objMaterial.vertexShader;
  auto fragmentShader = m_objMaterial.fragmentShader;

  if (vertexShader.isValid())
  {
    m_deviceFuncs->vkDestroyShaderModule(m_device, vertexShader.getData()->shaderModule, nullptr);
    vertexShader.reset();
  }

  if (fragmentShader.isValid())
  {
    m_deviceFuncs->vkDestroyShaderModule(m_device, fragmentShader.getData()->shaderModule, nullptr);
    fragmentShader.reset();
  }
}
