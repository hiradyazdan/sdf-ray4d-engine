/*****************************************************
 * Partial Class: DescriptorHelper
 * Members: Create Descriptor Set Helpers (Private)
 *****************************************************/

#include "Helpers/Descriptor.hpp"

using namespace sdfRay4d::helpers;

/**
 *
 * @param[in] _material
 * @param[out] _material->descPool
 */
void DescriptorHelper::createDescriptorPool(
  const MaterialPtr &_material
)
{
  // TODO: This should be set per material pipeline
  descriptor::PoolSize descPoolSizes[] = {
    {
      VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC,
      2
    }
  };
  descriptor::PoolInfo descPoolInfo = {}; // memset
  descPoolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
  /**
   * @note
   * a single set if dynamic uniform buffer is used
   * multiple set if texture sampler needed along with other descriptors (one set per texture/sampler)
   */
  descPoolInfo.maxSets = _material->descSetLayoutCount;
  descPoolInfo.poolSizeCount = sizeof(descPoolSizes) / sizeof(descPoolSizes[0]);
  descPoolInfo.pPoolSizes = descPoolSizes;

  auto result = m_deviceFuncs->vkCreateDescriptorPool(
    m_device,
    &descPoolInfo,
    nullptr,
    &_material->descPool
  );

  if (result != VK_SUCCESS)
  {
    qFatal("Failed to create descriptor pool: %d", result);
  }
}

/**
 *
 * @param[in] _material
 * @param[out] _material->descSetLayout
 */
void DescriptorHelper::createDescriptorSetLayout(
  const MaterialPtr &_material
)
{
  /**
   * @note
   *
   * In case need to use multiple different descriptor sets/layouts as
   * texture (combined image) sampler cannot be set using one single
   * dynamic uniform buffers along with other descriptors/UBOs.
   */
  auto &layoutBindings = _material->layoutBindings;
  auto &layoutCount = _material->descSetLayoutCount;

  _material->descSetLayouts.resize(layoutCount);

  for(auto i = 0; i < layoutCount; i++)
  {
    descriptor::LayoutInfo descLayoutInfo = {}; // memset
    descLayoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    descLayoutInfo.pNext = nullptr;
    descLayoutInfo.flags = 0;
    descLayoutInfo.bindingCount = layoutBindings.size();
    descLayoutInfo.pBindings = layoutBindings.data();

    auto result = m_deviceFuncs->vkCreateDescriptorSetLayout(
      m_device,
      &descLayoutInfo, // [in]
      nullptr,
      &_material->descSetLayouts[i] // [out] e.g. Texture Sampler Set Layout
    );

    if (result != VK_SUCCESS)
    {
      qFatal("Failed to create descriptor set layout: %d", result);
    }
  }
}

/**
 *
 * @param[in] _material
 * @param[out] _material->descSet
 */
void DescriptorHelper::allocateDescriptorSets(
  const MaterialPtr &_material
)
{
  /**
   * @note
   *
   * In case need to use multiple different descriptor sets as
   * texture (combined image) sampler cannot be set using one single
   * dynamic uniform buffers along with other descriptors/UBOs.
   */
  auto &layoutCount = _material->descSetLayoutCount;

  _material->descSets.resize(layoutCount);

  for(auto i = 0; i < layoutCount; i++)
  {
    descriptor::AllocInfo descSetInfo = {}; // memset
    descSetInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    descSetInfo.pNext = nullptr;
    descSetInfo.descriptorPool = _material->descPool;
    descSetInfo.descriptorSetCount = 1;
    descSetInfo.pSetLayouts = _material->descSetLayouts.data();

    auto result = m_deviceFuncs->vkAllocateDescriptorSets(
      m_device,
      &descSetInfo, // [in]
      &_material->descSets[i] // [out]
    );

    if (result != VK_SUCCESS)
    {
      qFatal("Failed to allocate descriptor set: %d", result);
    }
  }
}

/**
 *
 * @brief creates descriptor sets and layouts for the corresponding pipeline
 * @note per material pipeline
 *
 * @param[in] _material
 * @param[out] _material->descPool
 * @param[out] _material->descSetLayouts
 * @param[out] _material->descSets
 */
void DescriptorHelper::createDescriptorSets(
  const MaterialPtr &_material
)
{
  createDescriptorPool(_material);
  createDescriptorSetLayout(_material);
  allocateDescriptorSets(_material);
}
