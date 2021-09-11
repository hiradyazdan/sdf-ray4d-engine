#pragma once

#include "BaseHelper.hpp"

namespace sdfRay4d::helpers
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
      );

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
      );

    public:
      void allocateMemory(
        const device::Size &_size,
        uint32_t _typeIndex
      );
      void bindBufferMemory(
        const Buffer &_buffer,
        const device::Size &_memOffset
      );
      void mapMemory(size_t _byteSize);

    private:
      void destroyBuffer(Buffer &_buffer);
      void freeMemory();

    private:
      Device m_device = VK_NULL_HANDLE;
      QVulkanDeviceFunctions *m_deviceFuncs = VK_NULL_HANDLE;

//      std::vector<Buffer> m_buffers;
      device::Memory m_bufferMemory = VK_NULL_HANDLE;
  };
}
