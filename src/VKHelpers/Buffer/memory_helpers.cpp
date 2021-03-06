/*****************************************************
 * Partial Class: BufferHelper
 * Members: Memory Helpers (Private)
 *****************************************************/

#include "VKHelpers/Buffer.hpp"

using namespace sdfRay4d::vkHelpers;

/**
 *
 * @param[in] _size
 * @param[in] _typeIndex
 */
void BufferHelper::allocateMemory(
  const device::Size &_size,
  uint32_t _typeIndex
) noexcept
{
  memory::AllocInfo memAllocInfo = {
    memory::StructureType::MEMORY_ALLOC_INFO, // sType
    nullptr, // pNext
    _size, // allocationSize
    _typeIndex // memoryTypeIndex
  };
  auto result = m_deviceFuncs->vkAllocateMemory(
    m_device,
    &memAllocInfo,
    nullptr,
    &m_bufferMemory
  );

  if (result != VK_SUCCESS)
  {
    qFatal("Failed to allocate memory: %d", result);
  }
}

/**
 *
 * @param[in] _buffer
 * @param[in] _memOffset
 */
void BufferHelper::bindBufferMemory(
  const buffer::Buffer &_buffer,
  const device::Size &_memOffset
) noexcept
{
  auto result = m_deviceFuncs->vkBindBufferMemory(
    m_device,
    _buffer,
    m_bufferMemory,
    _memOffset
  );

  if (result != VK_SUCCESS)
  {
    qFatal("Failed to bind buffer memory: %d", result);
  }
}

/**
 *
 * @param[in] _byteSize
 */
void BufferHelper::mapMemory(
  size_t _byteSize
) noexcept
{
  // Copy vertex data.
  quint8 *p;
  auto result = m_deviceFuncs->vkMapMemory(
    m_device,
    m_bufferMemory,
    0,
    VK_WHOLE_SIZE/*m_actorMaterial->uniMemStartOffset*/,
    0,
    reinterpret_cast<void**>(&p)
  );

  if (result != VK_SUCCESS)
  {
    qFatal("Failed to map memory: %d", result);
  }

//  memcpy(p, , _byteSize);
//  //  memcpy(p + m_sdfUniformStartOffset, m_sdfUniformBuffer, );
  m_deviceFuncs->vkUnmapMemory(m_device, m_bufferMemory);
}

void BufferHelper::freeMemory() noexcept
{
  if (!m_bufferMemory) return;

  m_deviceFuncs->vkFreeMemory(
    m_device,
    m_bufferMemory,
    nullptr
  );
  m_bufferMemory = VK_NULL_HANDLE;
}
