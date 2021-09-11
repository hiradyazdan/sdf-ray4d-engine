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
        Device &_device,
        QVulkanDeviceFunctions *_deviceFuncs
//        const QString &_texturesPath = QStringLiteral("textures/"),
      );

    public:
      void createImage(
        uint32_t _width,
        uint32_t _height,
        texture::ImageUsage _usage =
          VK_IMAGE_USAGE_TRANSFER_SRC_BIT |
          VK_IMAGE_USAGE_STORAGE_BIT
      );
      void createImageMemory(uint32_t _deviceMemIndex);
      void createImageMemoryBarrier(
        const texture::ImageLayout &_oldLayout = VK_IMAGE_LAYOUT_UNDEFINED,
        const texture::ImageLayout &_newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        const texture::ImageAspect &_aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
        const texture::AccessFlags &_srcAccessMask = VK_ACCESS_NONE_KHR,
        const texture::AccessFlags &_destAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT
      );
      void createImageView(
        texture::ImageAspect _aspectMask = VK_IMAGE_ASPECT_COLOR_BIT
      );
      void createImageView(
        texture::ImageAspect _aspectMask,
        texture::ImageView &_imageView
      );
      void createSampler(
        const float &_maxAnisotropy = 1.0f,
        bool _enableAnisotropy = false
      );

    public:
      texture::Image &getImage() { return m_image; }
      texture::ImageView &getImageView() { return m_imageView; }
      texture::ImageMemoryBarrier &getImageMemoryBarrier() { return m_imageMemBarrier; }
      texture::Sampler &getSampler() { return m_sampler; }

    public:
      void destroy();

    /**
     * Qt Vulkan Members
     */
    private:
      Device m_device = VK_NULL_HANDLE;
      QVulkanDeviceFunctions *m_deviceFuncs = VK_NULL_HANDLE;

      texture::Image m_image = VK_NULL_HANDLE;
      texture::ImageView m_imageView = VK_NULL_HANDLE;
      texture::Sampler m_sampler = VK_NULL_HANDLE;
      texture::ImageMemoryBarrier m_imageMemBarrier = {};

      device::Memory m_imageMem = VK_NULL_HANDLE;
  };
}
