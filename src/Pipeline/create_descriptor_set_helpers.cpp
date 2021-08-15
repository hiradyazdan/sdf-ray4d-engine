/*****************************************************
 * Partial Class: Pipeline
 * Members: Create Descriptor Set Helpers (Private)
 *****************************************************/

#include "Pipeline.hpp"

using namespace sdfRay4d;

/**
 *
 * @param[in] _material
 * @param[out] _material->descPool
 */
void PipelineHelper::createDescriptorPool(
  const MaterialPtr &_material
)
{
  descriptor::PoolSize descPoolSizes[] = {
    {
      VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC,
      2
    }
  };
  descriptor::PoolInfo descPoolInfo = {};
  descPoolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
  /**
   * a single set if dynamic uniform buffer is used
   * multiple set if texture sampler needed along with other descriptors (one per texture/sampler)
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
void PipelineHelper::createDescriptorSetLayout(
  const MaterialPtr &_material
)
{
  auto &layoutBindings = _material->layoutBindings;
  auto &layoutCount = _material->descSetLayoutCount;
  auto isMultiSet = layoutCount > 1;

  if(isMultiSet)
  {
    _material->descSetLayouts = std::make_unique<descriptor::Layout[]>(layoutCount);
  }

  /**
   * @note
   *
   * In case need to use multiple different descriptor sets/layouts as
   * texture (combined image) sampler cannot be set using one single
   * dynamic uniform buffers along with other descriptors/UBOs.
   *
   * This is the case for passing depth color from rasterization pass
   * to sdf raymarching pass through a texture/combined image sampler
   * (depth texture)
   */
  for(auto index = 0; index < layoutCount; index++)
  {
    descriptor::LayoutInfo descLayoutInfo = {
      VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO, // sType
      nullptr, // pNext
      0, // flags
      static_cast<uint32_t>(layoutBindings.size()),//sizeof(layoutBindings) / sizeof(layoutBindings[0]), // bindingCount
//      layoutCount > 1
//      ? &layoutBindings[index]
      layoutBindings.data() // pBindings
    };

    auto result = m_deviceFuncs->vkCreateDescriptorSetLayout(
      m_device,
      &descLayoutInfo,
      nullptr,
      &_material->descSetLayout // e.g. UBO or other Desc Set Layout + Texture Sampler Set Layout
    );

    if (result != VK_SUCCESS)
    {
      qFatal("Failed to create descriptor set layout: %d", result);
    }

    if(isMultiSet)
    {
      _material->descSetLayouts[index] = _material->descSetLayout;
    }
  }
}

/**
 *
 * @param[in] _material
 * @param[out] _material->descSet
 */
void PipelineHelper::allocateDescriptorSets(
  const MaterialPtr &_material
)
{
  auto &layoutCount = _material->descSetLayoutCount;
  auto isMultiSet = layoutCount > 1;

  if(isMultiSet)
  {
    _material->descSets = std::make_unique<descriptor::Set[]>(layoutCount);
  }

  /**
   * @note
   *
   * In case need to use multiple different descriptor sets as
   * texture (combined image) sampler cannot be set using one single
   * dynamic uniform buffers along with other descriptors/UBOs.
   *
   * This is the case for passing depth color from rasterization pass
   * to sdf raymarching pass through a texture/combined image sampler
   * (depth texture)
   */
  for(auto index = 0; index < layoutCount; index++)
  {
    descriptor::AllocInfo descSetAllocInfo = {
      VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
      nullptr,
      _material->descPool,
      1,
      layoutCount > 1
      ? _material->descSetLayouts.get()
      : &_material->descSetLayout
    };

    auto result = m_deviceFuncs->vkAllocateDescriptorSets(
      m_device,
      &descSetAllocInfo,
      &_material->descSet
    );

    if (result != VK_SUCCESS)
    {
      qFatal("Failed to allocate descriptor set: %d", result);
    }

    if(isMultiSet)
    {
      _material->descSets[index] = _material->descSet;
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
 * @param[out] _material->descSetLayout
 * @param[out] _material->descSet
 */
void PipelineHelper::createDescriptorSets(
  const MaterialPtr &_material
)
{
  createDescriptorPool(_material);
  createDescriptorSetLayout(_material);
  allocateDescriptorSets(_material);
}
