/*****************************************************
 * Partial Class: BufferHelper (Ctor & General)
 * Members: General Functions (Public/Private)
 *
 * This Class is split into partials to categorize
 * and classify the functionality
 * for the purpose of readability/maintainability
 *
 * The partials can be found in the respective
 * directory named as the class name
 *
 * Partials:
 * - create_buffer_helpers.cpp
 * - memory_helpers.cpp
 *****************************************************/

#include "Helpers/Buffer.hpp"

using namespace sdfRay4d::helpers;

BufferHelper::BufferHelper(
  const Device &_device,
  QVulkanDeviceFunctions *_deviceFuncs
) noexcept :
  m_device(_device)
, m_deviceFuncs(_deviceFuncs)
{}

void BufferHelper::destroyBuffer(Buffer &_buffer) noexcept
{
  if (!_buffer) return;

  m_deviceFuncs->vkDestroyBuffer(
    m_device,
    _buffer,
    nullptr
  );
  _buffer = VK_NULL_HANDLE;
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
