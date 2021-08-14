/*****************************************************
 * Partial Class: Texture (General)
 * Members: General Functions (Public)
 * Partials: None
 *****************************************************/

#include "Texture.hpp"

using namespace sdfRay4d;

Texture::Texture(
  Device &_device,
  QVulkanDeviceFunctions *_deviceFuncs
//  const QString &_texturesPath
  ) :
  m_device(_device)
  , m_deviceFuncs(_deviceFuncs)
//  , m_texturesPath(m_appConstants.assetsPath + _texturesPath)
{}

void Texture::createImage(uint32_t _width, uint32_t _height)
{
  texture::ImageInfo imageInfo = {}; // memset

  imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
  imageInfo.imageType = VK_IMAGE_TYPE_2D;
  imageInfo.extent.width = _width;
  imageInfo.extent.height = _height;
  imageInfo.extent.depth = 1;
  imageInfo.mipLevels = 1;
  imageInfo.arrayLayers = 1;
  imageInfo.format = VK_FORMAT_R8G8B8A8_SRGB;
  imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
  imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
  imageInfo.usage = VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_STORAGE_BIT;
  imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
  imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

  auto result = m_deviceFuncs->vkCreateImage(
    m_device,
    &imageInfo,
    nullptr,
    &m_image
  );

  if (result != VK_SUCCESS)
  {
    qFatal("Failed to create Texture Image: %d", result);
  }
}

void Texture::createImageView()
{
  texture::ImageViewInfo viewInfo = {}; // memset

  viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
  viewInfo.image = m_image;
  viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
  viewInfo.format = VK_FORMAT_R8G8B8A8_SRGB;
  viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
  viewInfo.subresourceRange.baseMipLevel = 0;
  viewInfo.subresourceRange.levelCount = 1;
  viewInfo.subresourceRange.baseArrayLayer = 0;
  viewInfo.subresourceRange.layerCount = 1;

  auto result = m_deviceFuncs->vkCreateImageView(
    m_device,
    &viewInfo,
    nullptr,
    &m_imageView
  );

  if (result != VK_SUCCESS)
  {
    qFatal("Failed to create Texture Image View: %d", result);
  }
}

void Texture::createSampler(const float &_maxAnisotropy, bool enableAnisotropy)
{
  texture::SamplerInfo samplerInfo = {}; // memset

  samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;

  samplerInfo.magFilter = VK_FILTER_LINEAR;
  samplerInfo.minFilter = VK_FILTER_LINEAR;

  samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
  samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
  samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;

  samplerInfo.anisotropyEnable = enableAnisotropy ? VK_TRUE : VK_FALSE;
  samplerInfo.maxAnisotropy = _maxAnisotropy;

  samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;

  samplerInfo.unnormalizedCoordinates = VK_FALSE;

  samplerInfo.compareEnable = VK_FALSE;
  samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;

  // mipmapping
  samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
  samplerInfo.mipLodBias = 0.0f;
  samplerInfo.minLod = 0.0f;
  samplerInfo.maxLod = 0.0f;

  auto result = m_deviceFuncs->vkCreateSampler(
    m_device,
    &samplerInfo,
    nullptr,
    &m_sampler
  );

  if (result != VK_SUCCESS)
  {
    qFatal("Failed to create Texture Sampler: %d", result);
  }
}

void Texture::destroy()
{
  m_deviceFuncs->vkDestroySampler(m_device, m_sampler, nullptr);

  m_deviceFuncs->vkDestroyImageView(m_device, m_imageView, nullptr);
  m_deviceFuncs->vkDestroyImage(m_device, m_image, nullptr);

  m_deviceFuncs->vkFreeMemory(m_device, m_imageMem, nullptr);
}
