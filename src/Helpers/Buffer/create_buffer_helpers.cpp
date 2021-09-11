/*****************************************************
 * Partial Class: BufferHelper
 * Members: Create Buffer Helpers (Private)
 *****************************************************/

#include "Helpers/Buffer.hpp"

using namespace sdfRay4d::helpers;

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
  Buffer &_buffer,
  memory::Reqs &_memReq
)
{
  buffer::Info bufInfo = {}; // memset
  bufInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
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
