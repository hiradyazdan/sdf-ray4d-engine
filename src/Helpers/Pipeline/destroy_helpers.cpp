/*****************************************************
 * Partial Class: PipelineHelper
 * Members: Destroy Helpers (Public)
 *****************************************************/

#include "Helpers/Pipeline.hpp"

using namespace sdfRay4d::helpers;

void PipelineHelper::destroyPipeline(Pipeline &_pipeline)
{
  if (!_pipeline) return;

  m_deviceFuncs->vkDestroyPipeline(
    m_device,
    _pipeline,
    nullptr
  );
  _pipeline = VK_NULL_HANDLE;
}

void PipelineHelper::destroyPipelineLayout(
  pipeline::Layout &_pipelineLayout
)
{
  if (!_pipelineLayout) return;

  m_deviceFuncs->vkDestroyPipelineLayout(
    m_device,
    _pipelineLayout,
    nullptr
  );
  _pipelineLayout = VK_NULL_HANDLE;
}

void PipelineHelper::destroyPipeline(
  const MaterialPtr &_material
)
{
  destroyPipeline(_material->pipeline);
}

void PipelineHelper::destroyPipelineLayout(
  const MaterialPtr &_material
)
{
  destroyPipelineLayout(_material->pipelineLayout);
}

void PipelineHelper::destroyPipelines()
{
  for(const auto &material : m_materials)
  {
    destroyPipelineLayout(material);
    destroyPipeline(material);
  }

  if (!m_pipelineCache) return;

  m_deviceFuncs->vkDestroyPipelineCache(
    m_device,
    m_pipelineCache,
    nullptr
  );
  m_pipelineCache = VK_NULL_HANDLE;
}

void PipelineHelper::destroyDescriptorSetLayout(DescLayoutList &_setLayouts)
{
  if (_setLayouts.empty()) return;

  for (auto &layout : _setLayouts)
  {
    m_deviceFuncs->vkDestroyDescriptorSetLayout(
      m_device,
      layout,
      nullptr
    );
  }

  _setLayouts.resize(0);
}

void PipelineHelper::destroyDescriptorPool(
  descriptor::Pool &_pool
)
{
  if (!_pool) return;

  m_deviceFuncs->vkDestroyDescriptorPool(
    m_device,
    _pool,
    nullptr
  );
  _pool = VK_NULL_HANDLE;
}

void PipelineHelper::destroyDescriptors()
{
  for(const auto &material : m_materials)
  {
    destroyDescriptorSetLayout(material->descSetLayouts);
    destroyDescriptorPool(material->descPool);
  }
}

void PipelineHelper::destroyShaderModule(Shader &_shader)
{
  if (!_shader.isValid()) return;

  m_deviceFuncs->vkDestroyShaderModule(
    m_device,
    _shader.getData()->shaderModule,
    nullptr
  );

  _shader.reset();
}

void PipelineHelper::destroyShaderModules()
{
  for(const auto &material : m_materials)
  {
    destroyShaderModule(material->vertexShader);
    destroyShaderModule(material->fragmentShader);
  }
}

void PipelineHelper::destroyTexture(Texture &_texture)
{
  _texture.destroy();
}

void PipelineHelper::destroyTextures()
{
  for(const auto &material : m_materials)
  {
    material->texture.destroy();
  }
}

void PipelineHelper::destroyRenderPass()
{
  for (const auto &material : m_materials)
  {
    auto &framebuffer = material->framebuffer;
    auto &renderPass = material->renderPass;

    if(framebuffer && !material->isDefault)
    {
      m_deviceFuncs->vkDestroyFramebuffer(
        m_device,
        framebuffer,
        nullptr
      );
      framebuffer = VK_NULL_HANDLE;
    }

    if(renderPass && !material->isDefault)
    {
      m_deviceFuncs->vkDestroyRenderPass(
        m_device,
        renderPass,
        nullptr
      );
      renderPass = VK_NULL_HANDLE;
    }
  }
}

void PipelineHelper::destroyBuffers()
{
  for(auto &material : m_materials)
  {
    buffer.destroyBuffer(material->buffer);
    buffer.destroyBuffer(material->dynamicUniformBuffer);
    buffer.freeMemory();
  }
}
