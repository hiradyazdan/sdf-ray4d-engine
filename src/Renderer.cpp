/*****************************************************
 * Partial Class: Renderer (Ctor & General)
 * Members: General Functions (Public/Private)
 *
 * This Class is split into partials to categorize
 * and classify the functionality
 * for the purpose of readability/maintainability
 *
 * The partials can be found in the respective
 * directory named as the class name
 *
 * Partials:
 * - resources (resources.cpp)
 *      - destroy_helpers.cpp
 *      - init_helpers.cpp
 *      - init_materials_helpers.cpp
 *      - init_shaders_helpers.cpp
 *      - sdf_graph_pipeline_helpers.cpp
 * - frame (frame.cpp)
 *      - buffers.cpp
 *      - draw_calls.cpp
 *      - render_pass.cpp
 *      - user_input_helpers.cpp
 * - swapchain_resources.cpp
 *****************************************************/

#include "Renderer.hpp"

using namespace sdfRay4d;

Renderer::Renderer(
  VulkanWindow *_vkWindow,
  bool _isMSAA
) :
  m_vkWindow(_vkWindow)
  , m_isMSAA(_isMSAA)
{
  m_actorMesh.load(m_appConstants.assetsPath + "models/" + m_appConstants.modelsPaths.actor);

  connect(
    &m_frameWatcher, &QFutureWatcherBase::finished,
    this, &Renderer::updateFrame
  );
}

void Renderer::markViewProjDirty()
{
  m_vpDirty = m_vkWindow->concurrentFrameCount();
}

/**
 *
 */
void Renderer::createDepthImageView()
{
  auto windowSize = m_vkWindow->swapChainImageSize();
  auto extentWidth = uint32_t(windowSize.width());
  auto extentHeight = uint32_t(windowSize.height());

  texture::ImageInfo image_info = {};

  image_info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
  image_info.pNext = nullptr;
  image_info.imageType = VK_IMAGE_TYPE_2D;
  image_info.format = VK_FORMAT_D16_UNORM;
  image_info.extent.width = extentWidth;
  image_info.extent.height = extentHeight;
  image_info.extent.depth = 1;
  image_info.mipLevels = 1;
  image_info.arrayLayers = 1;
  image_info.samples = VK_SAMPLE_COUNT_1_BIT;
  image_info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
  image_info.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;//VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
  image_info.queueFamilyIndexCount = 0;
  image_info.pQueueFamilyIndices = nullptr;
  image_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
  image_info.flags = 0;

  m_deviceFuncs->vkCreateImage(m_device, &image_info, nullptr, &m_image);
  m_deviceFuncs->vkGetImageMemoryRequirements(m_device, m_image, &memReqs);

  VkMemoryAllocateInfo mem_alloc = {};
  mem_alloc.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
  mem_alloc.pNext = nullptr;
  mem_alloc.allocationSize = memReqs.size;
  mem_alloc.memoryTypeIndex = m_vkWindow->deviceLocalMemoryIndex();//memory_type_from_properties();

  m_deviceFuncs->vkAllocateMemory(
    m_device,
    &mem_alloc,
    nullptr,
    &imageMem
  );

  m_deviceFuncs->vkBindImageMemory(
    m_device,
    m_image,
    imageMem,
    0
  );




  VkImageViewCreateInfo view_info = {};
  view_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
  view_info.pNext = nullptr;
  view_info.image = m_image;
  view_info.format = VK_FORMAT_D16_UNORM;
  view_info.components.r = VK_COMPONENT_SWIZZLE_R;
  view_info.components.g = VK_COMPONENT_SWIZZLE_G;
  view_info.components.b = VK_COMPONENT_SWIZZLE_B;
  view_info.components.a = VK_COMPONENT_SWIZZLE_A;
  view_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
  view_info.subresourceRange.baseMipLevel = 0;
  view_info.subresourceRange.levelCount = 1;
  view_info.subresourceRange.baseArrayLayer = 0;
  view_info.subresourceRange.layerCount = 1;
  view_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
  view_info.flags = 0;

  auto result = m_deviceFuncs->vkCreateImageView(
    m_device,
    &view_info,
    nullptr,
    &m_imageView
  );

  if (result != VK_SUCCESS)
  {
    qFatal("Failed to create image view: %d", result);
  }
}
