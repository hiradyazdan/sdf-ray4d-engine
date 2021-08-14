/*****************************************************
 * Partial Class: Renderer
 * Members: Destroy Resources helpers (Public/Private)
 *****************************************************/

#include "Renderer.hpp"

using namespace sdfRay4d;

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

void Renderer::destroyShaderModules()
{
  for(const auto &material : m_materials)
  {
    destroyShaderModule(material->vertexShader);
    destroyShaderModule(material->fragmentShader);
  }
}

void Renderer::destroyBuffer(Buffer &_buffer)
{
  if (_buffer)
  {
    m_deviceFuncs->vkDestroyBuffer(
      m_device,
      _buffer,
      nullptr
    );
    _buffer = VK_NULL_HANDLE;
  }
}

void Renderer::destroyBuffers()
{
  for(auto &buffer : m_buffers)
  {
    destroyBuffer(buffer);
  }

  if (m_bufferMemory)
  {
    m_deviceFuncs->vkFreeMemory(
      m_device,
      m_bufferMemory,
      nullptr
    );
    m_bufferMemory = VK_NULL_HANDLE;
  }
}
