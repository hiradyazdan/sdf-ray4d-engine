/*****************************************************
 * Partial Class: Renderer
 * Members: Destroy Resources helpers (Private)
 *****************************************************/

#include "Renderer.hpp"

using namespace sdfRay4d;

void Renderer::destroyPipeline()
{
  if (m_pipeline)
  {
    m_deviceFuncs->vkDestroyPipeline(m_device, m_pipeline, nullptr);
    m_pipeline = VK_NULL_HANDLE;
  }

  if (m_pipelineLayout)
  {
    m_deviceFuncs->vkDestroyPipelineLayout(m_device, m_pipelineLayout, nullptr);
    m_pipelineLayout = VK_NULL_HANDLE;
  }

  if (m_pipelineCache)
  {
    m_deviceFuncs->vkDestroyPipelineCache(m_device, m_pipelineCache, nullptr);
    m_pipelineCache = VK_NULL_HANDLE;
  }
}

void Renderer::destroyDescriptorSets()
{
  if (m_descSetLayout)
  {
    m_deviceFuncs->vkDestroyDescriptorSetLayout(m_device, m_descSetLayout, nullptr);
    m_descSetLayout = VK_NULL_HANDLE;
  }

  if (m_descPool)
  {
    m_deviceFuncs->vkDestroyDescriptorPool(m_device, m_descPool, nullptr);
    m_descPool = VK_NULL_HANDLE;
  }
}

void Renderer::destroyBuffers()
{
  if (m_buf)
  {
    m_deviceFuncs->vkDestroyBuffer(m_device, m_buf, nullptr);
    m_buf = VK_NULL_HANDLE;
  }

  if (m_bufMem)
  {
    m_deviceFuncs->vkFreeMemory(m_device, m_bufMem, nullptr);
    m_bufMem = VK_NULL_HANDLE;
  }
}
