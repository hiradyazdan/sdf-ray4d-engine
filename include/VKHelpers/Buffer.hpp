#pragma once

#include "BaseHelper.hpp"

namespace sdfRay4d::vkHelpers
{
  /**
   * @class BufferHelper
   * @brief
   *
   * @example
   *
   */
  class BufferHelper : protected BaseHelper
  {
    friend class PipelineHelper;

    public:
      BufferHelper(
        const Device &_device,
        QVulkanDeviceFunctions *_deviceFuncs
      ) noexcept;

    /**
     * @note BufferHelper is non-copyable
     */
    public:
      BufferHelper() = default;
      BufferHelper(const BufferHelper&) = delete;

    public:
      void createBuffer(
        device::Size _size,
        const buffer::UsageFlags &_usage,
        Buffer &_buffer,
        memory::Reqs &_memReq
      ) noexcept;

    public:
      void allocateMemory(
        const device::Size &_size,
        uint32_t _typeIndex
      ) noexcept;
      void bindBufferMemory(
        const Buffer &_buffer,
        const device::Size &_memOffset
      ) noexcept;
      void mapMemory(size_t _byteSize) noexcept;

    private:
      void destroyBuffer(Buffer &_buffer) noexcept;
      void freeMemory() noexcept;

    private:
      Device m_device = VK_NULL_HANDLE;
      QVulkanDeviceFunctions *m_deviceFuncs = VK_NULL_HANDLE;

//      std::vector<Buffer> m_buffers;
      device::Memory m_bufferMemory = VK_NULL_HANDLE;
  };
}
