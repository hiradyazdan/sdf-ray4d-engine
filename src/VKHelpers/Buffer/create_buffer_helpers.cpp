/*****************************************************
 * Partial Class: BufferHelper
 * Members: Create Buffer Helpers (Private)
 *****************************************************/

#include "VKHelpers/Buffer.hpp"

using namespace sdfRay4d::vkHelpers;

/**
 *
 * @param[in] _size
 * @param[in] _usage
 * @param[in, out] _buffer
 * @param[in, out] _memReq
 */
void BufferHelper::createBuffer(
  device::Size _size,
  const buffer::UsageFlags &_usage,
  buffer::Buffer &_buffer,
  memory::Reqs &_memReq
) noexcept
{
  buffer::Info bufInfo = {}; // memset
  bufInfo.sType = buffer::StructureType::BUFFER_INFO;
  bufInfo.size = _size;
  bufInfo.usage = _usage;

  auto result = m_deviceFuncs->vkCreateBuffer(
    m_device,
    &bufInfo,
    nullptr,
    &_buffer
  );

  if (result != VK_SUCCESS)
  {
    qFatal("Failed to create buffer: %d", result);
  }

  m_deviceFuncs->vkGetBufferMemoryRequirements(
    m_device,
    _buffer,
    &_memReq
  );
}
