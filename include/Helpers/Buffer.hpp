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
    public:
      BufferHelper();

    public:
      void createBuffers();
      void createBuffer(
        device::Size _size,
        buffer::UsageFlags _usage,
        Buffer &_buffer,
        memory::Reqs &_memReq
      );
      void allocateMemory();
      void bindBufferToMemory(
        const Buffer &_buffer,
        const device::Size &_memOffset
      );
      void mapMemory(size_t _byteSize);
      void updateDescriptorSets();
  };
}
