/*****************************************************
 * Partial Class: DescriptorHelper
 * Members: Create Descriptor Set Helpers (Private)
 *****************************************************/

#include "VKHelpers/Descriptor.hpp"

using namespace sdfRay4d::vkHelpers;

/**
 *
 * @param[in] _material
 * @param[out] _material->descPool
 */
void DescriptorHelper::createDescriptorPool(
  const MaterialPtr &_material
) noexcept
{
  descriptor::PoolInfo descPoolInfo = {}; // memset
  descPoolInfo.sType = descriptor::StructureType::POOL_INFO;
  /**
   * @note
   * a single set if dynamic uniform buffer is used
   * multiple set if texture sampler needed along with other descriptors (one set per texture/sampler)
   */
  descPoolInfo.maxSets = _material->descSetLayoutCount;
  descPoolInfo.poolSizeCount = _material->descPoolSizes.size();
  descPoolInfo.pPoolSizes = _material->descPoolSizes.data();

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
 * @param[out] _material->descSetLayouts
 */
void DescriptorHelper::createDescriptorSetLayouts(
  const MaterialPtr &_material
) noexcept
{
  /**
   * @note
   *
   * In case need to use multiple different descriptor sets/layouts as
   * texture (combined image) sampler cannot be set using one single
   * dynamic uniform buffers along with other descriptors/UBOs.
   */

  const auto &layoutBindings = _material->layoutBindings;
  const auto &layoutCount = _material->descSetLayoutCount;
  auto &layouts = _material->descSetLayouts;

  layouts.resize(layoutCount);

  for(auto i = 0; i < layoutCount; i++)
  {
    descriptor::LayoutInfo descLayoutInfo = {}; // memset
    descLayoutInfo.sType = descriptor::StructureType::SET_LAYOUT_INFO;
    descLayoutInfo.pNext = nullptr;
    descLayoutInfo.flags = 0;
    descLayoutInfo.bindingCount = layoutBindings.size();
    descLayoutInfo.pBindings = layoutBindings.data();

    auto result = m_deviceFuncs->vkCreateDescriptorSetLayout(
      m_device,
      &descLayoutInfo, // [in]
      nullptr,
      &layouts[i] // [out] e.g. Texture Sampler Set Layout
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
 * @param[out] _material->descSets
 */
void DescriptorHelper::allocateDescriptorSets(
  const MaterialPtr &_material
) noexcept
{
  /**
   * @note
   *
   * In case need to use multiple different descriptor sets as
   * texture (combined image) sampler cannot be set using one single
   * dynamic uniform buffers along with other descriptors/UBOs.
   */

  const auto &layoutCount = _material->descSetLayoutCount;
  const auto &layouts = _material->descSetLayouts;
  auto &sets = _material->descSets;

  sets.resize(layoutCount);

  descriptor::AllocInfo descSetInfo = {}; // memset
  descSetInfo.sType                 = descriptor::StructureType::SET_ALLOC_INFO;
  descSetInfo.pNext                 = nullptr;
  descSetInfo.descriptorPool        = _material->descPool;
  descSetInfo.descriptorSetCount    = layouts.size();
  descSetInfo.pSetLayouts           = layouts.data();

  auto result = m_deviceFuncs->vkAllocateDescriptorSets(
    m_device,
    &descSetInfo, // [in]
    sets.data() // [out]
  );

  if (result != VK_SUCCESS)
  {
    qFatal("Failed to allocate descriptor sets: %d", result);
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
) noexcept
{
  createDescriptorPool(_material);
  createDescriptorSetLayouts(_material);
  allocateDescriptorSets(_material);
}
