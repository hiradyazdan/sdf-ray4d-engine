#pragma once

namespace sdfRay4d
{
  /**
   * @class BufferHelper
   * @brief
   *
   * @example
   *
   */
  class BufferHelper
  {
    public:
      BufferHelper();

    public:
      void createBuffers();
      void createBuffer(
        Buffer &_buffer,
        device::Size _size,
        buffer::UsageFlags _usage
      );
  };
}
