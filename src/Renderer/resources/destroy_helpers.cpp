/*****************************************************
 * Partial Class: Renderer
 * Members: Destroy Resources helpers (Public/Private)
 *****************************************************/

#include "Renderer.hpp"

using namespace sdfRay4d;

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
    m_deviceFuncs->vkFreeMemory(m_device, m_bufferMemory,nullptr);
    m_bufferMemory = VK_NULL_HANDLE;
  }

  if(m_imageBufferMemory)
  {
    m_deviceFuncs->vkFreeMemory(m_device, m_imageBufferMemory,nullptr);
    m_imageBufferMemory = VK_NULL_HANDLE;
  }
}
