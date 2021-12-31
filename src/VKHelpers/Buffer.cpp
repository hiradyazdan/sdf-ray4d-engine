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

#include "VKHelpers/Buffer.hpp"

using namespace sdfRay4d::vkHelpers;

BufferHelper::BufferHelper(
  const device::Device &_device,
  QVulkanDeviceFunctions *_deviceFuncs
) noexcept :
  m_device(_device)
, m_deviceFuncs(_deviceFuncs)
{}

void BufferHelper::destroyBuffer(buffer::Buffer &_buffer) noexcept
{
  if (!_buffer) return;

  m_deviceFuncs->vkDestroyBuffer(
    m_device,
    _buffer,
    nullptr
  );
  _buffer = VK_NULL_HANDLE;
}
