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
      void createImage(uint32_t _width, uint32_t _height);
      void createImageView();
      void createSampler(
        const float &_maxAnisotropy = 1.0f,
        bool enableAnisotropy = false
      );

    public:
      texture::ImageView getImageView() { return m_imageView; }
      texture::Sampler getSampler() { return m_sampler; }

    public:
      void destroy();

    /**
     * Qt Vulkan Members
     */
    private:
      Device m_device = VK_NULL_HANDLE;
      QVulkanDeviceFunctions *m_deviceFuncs = VK_NULL_HANDLE;

      device::Memory m_imageMem;

      texture::Image m_image;
      texture::ImageView m_imageView;
      texture::Sampler m_sampler;
  };
}
