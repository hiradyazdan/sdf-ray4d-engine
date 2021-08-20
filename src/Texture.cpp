/*****************************************************
 * Partial Class: Texture (General)
 * Members: General Functions (Public)
 * Partials: None
 *****************************************************/

#include "Texture.hpp"

using namespace sdfRay4d;
using namespace texture;

Texture::Texture(
  Device &_device,
  QVulkanDeviceFunctions *_deviceFuncs
//  const QString &_texturesPath
  ) :
  m_device(_device)
  , m_deviceFuncs(_deviceFuncs)
//  , m_texturesPath(m_appConstants.assetsPath + _texturesPath)
{}

void Texture::createImage(
  uint32_t _width,
  uint32_t _height,
  ImageUsage _usage
)
{
  ImageInfo imageInfo = {}; // memset

  imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
  imageInfo.pNext = nullptr;
  imageInfo.imageType = VK_IMAGE_TYPE_2D;
  imageInfo.format = VK_FORMAT_D16_UNORM;
//  imageInfo.format = VK_FORMAT_R8G8B8A8_SRGB;
  imageInfo.extent.width = _width;
  imageInfo.extent.height = _height;
  imageInfo.extent.depth = 1;
  imageInfo.mipLevels = 1;
  imageInfo.arrayLayers = 1;
  imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
  imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
  imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
  imageInfo.usage = _usage;
  imageInfo.queueFamilyIndexCount = 0;
  imageInfo.pQueueFamilyIndices = nullptr;
  imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
  imageInfo.flags = 0;

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

/**
 *
 * @param[in] _deviceMemIndex
 * @param[in,out] _bufferMemory
 */
void Texture::createImageMemory(
  uint32_t _deviceMemIndex,
  device::Memory &_bufferMemory
)
{
  memory::Reqs memReqs;

  m_deviceFuncs->vkGetImageMemoryRequirements(
    m_device,
    m_image,
    &memReqs
  );

  memory::AllocInfo memAllocInfo = {};
  memAllocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
  memAllocInfo.pNext = nullptr;
  memAllocInfo.allocationSize = memReqs.size;
  memAllocInfo.memoryTypeIndex = _deviceMemIndex;

  m_deviceFuncs->vkAllocateMemory(
    m_device,
    &memAllocInfo,
    nullptr,
    &_bufferMemory
  );

  m_deviceFuncs->vkBindImageMemory(
    m_device,
    m_image,
    _bufferMemory,
    0
  );
}

void Texture::createImageView(
  ImageAspect _aspectMask
)
{
  createImageView(_aspectMask, &m_imageView);
}

/**
 * @param[in] _aspectMask
 * @param[in,out] _imageView (m_imageView)
 */
void Texture::createImageView(
  ImageAspect _aspectMask,
  texture::ImageView *_imageView
)
{
  ImageViewInfo viewInfo = {}; // memset

  viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
  viewInfo.pNext = nullptr;
  viewInfo.image = m_image;
  viewInfo.format = VK_FORMAT_D16_UNORM;//VK_FORMAT_R8G8B8A8_SRGB;

  viewInfo.components.r = VK_COMPONENT_SWIZZLE_R;
  viewInfo.components.g = VK_COMPONENT_SWIZZLE_G;
  viewInfo.components.b = VK_COMPONENT_SWIZZLE_B;
  viewInfo.components.a = VK_COMPONENT_SWIZZLE_A;

  viewInfo.subresourceRange.aspectMask = _aspectMask;
  viewInfo.subresourceRange.baseMipLevel = 0;
  viewInfo.subresourceRange.levelCount = 1;
  viewInfo.subresourceRange.baseArrayLayer = 0;
  viewInfo.subresourceRange.layerCount = 1;

  viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
  viewInfo.flags = 0;

  auto result = m_deviceFuncs->vkCreateImageView(
    m_device,
    &viewInfo,
    nullptr,
    _imageView
  );

  if (result != VK_SUCCESS)
  {
    qFatal("Failed to create Texture Image View: %d", result);
  }

  m_imageView = *_imageView;
}

void Texture::createSampler(
  const float &_maxAnisotropy,
  bool enableAnisotropy
)
{
  SamplerInfo samplerInfo = {}; // memset

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
