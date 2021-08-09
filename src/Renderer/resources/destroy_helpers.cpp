/*****************************************************
 * Partial Class: Renderer
 * Members: Destroy Resources helpers (Public/Private)
 *****************************************************/

#include "Renderer.hpp"

using namespace sdfRay4d;

void Renderer::destroySDFPipeline()
{
  if (m_isFramePending)
  {
    m_isFramePending = false;
    m_vkWindow->frameReady();
  }

  destroyPipeline(m_sdfMaterial);
  destroyPipelineLayout(m_sdfMaterial);
  destroyShaderModule(m_sdfMaterial->fragmentShader);
}

void Renderer::destroyPipeline(
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

void Renderer::destroyPipelineLayout(
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

void Renderer::destroyShaderModule(Shader &_shader)
{
  if (_shader.isValid())
  {
    m_deviceFuncs->vkDestroyShaderModule(
      m_device,
      _shader.getData()->shaderModule,
      nullptr
      );
    _shader.reset();
  }
}

void Renderer::destroyDescriptorSets()
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

void Renderer::destroyBuffers()
{
  if (m_uniformBuffer)
  {
    m_deviceFuncs->vkDestroyBuffer(
      m_device,
      m_uniformBuffer,
      nullptr
    );
    m_uniformBuffer = VK_NULL_HANDLE;
  }

  if (m_bufMem)
  {
    m_deviceFuncs->vkFreeMemory(
      m_device,
      m_bufMem,
      nullptr
    );
    m_bufMem = VK_NULL_HANDLE;
  }
}

void Renderer::destroyPipelines()
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

void Renderer::destroyShaderModules()
{
  for(const auto &material : m_materials)
  {
    destroyShaderModule(material->vertexShader);
    destroyShaderModule(material->fragmentShader);
  }
}
