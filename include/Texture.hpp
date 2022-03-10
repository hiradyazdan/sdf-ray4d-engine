#pragma once

#include "Types.hpp"

namespace sdfRay4d
{
  using namespace vk;

  /**
   * @class Texture
   * @brief
   *
   */
  class Texture
  {
    public:
      Texture(
        const device::Device &_device,
        QVulkanDeviceFunctions *_deviceFuncs
//        const QString &_texturesPath = QStringLiteral("textures/"),
      );

    public:
      void createImage(
        uint32_t _width,
        uint32_t _height,
        image::Usage _usage =
          VK_IMAGE_USAGE_TRANSFER_SRC_BIT |
          VK_IMAGE_USAGE_STORAGE_BIT
      );
      void createImageMemory(uint32_t _deviceMemIndex);
      void createImageMemoryBarrier(
        const image::Layout &_oldLayout = VK_IMAGE_LAYOUT_UNDEFINED,
        const image::Layout &_newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        const image::Aspect &_aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
        const memory::AccessFlags &_srcAccessMask = VK_ACCESS_NONE_KHR,
        const memory::AccessFlags &_destAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT
      );
      void createImageView(
        image::Aspect _aspectMask = VK_IMAGE_ASPECT_COLOR_BIT
      );
      void createImageView(
        image::Aspect _aspectMask,
        image::View &_imageView
      );
      void createSampler(
        const float &_maxAnisotropy = 1.0f,
        bool _enableAnisotropy = false
      );

    public:
      image::Image &getImage() { return m_image; }
      image::View &getImageView() { return m_imageView; }
      image::MemBarrier &getImageMemoryBarrier() { return m_imageMemBarrier; }
      image::Sampler &getSampler() { return m_sampler; }

    public:
      void destroy();

    /**
     * Qt Vulkan Members
     */
    private:
      device::Device m_device = VK_NULL_HANDLE;
      QVulkanDeviceFunctions *m_deviceFuncs = VK_NULL_HANDLE;

      image::Image m_image = VK_NULL_HANDLE;
      image::View m_imageView = VK_NULL_HANDLE;
      image::Sampler m_sampler = VK_NULL_HANDLE;
      image::MemBarrier m_imageMemBarrier = {};

      device::Memory m_imageMem = VK_NULL_HANDLE;
  };
}
