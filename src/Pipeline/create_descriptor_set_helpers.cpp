/*****************************************************
 * Partial Class: Pipeline
 * Members: Create Descriptor Helpers (Private)
 *****************************************************/

#include "Pipeline.hpp"

using namespace sdfRay4d;

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
  descPoolInfo.maxSets = 1; // a single set is enough due to the dynamic uniform buffer
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

void PipelineHelper::createDescriptorSetLayout(
  const MaterialPtr &_material
)
{
  auto &layoutBindings = _material->layoutBindings;

  descriptor::LayoutInfo descLayoutInfo = {
    VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO, // sType
    nullptr, // pNext
    0, // flags
    sizeof(layoutBindings) / sizeof(layoutBindings[0]), // bindingCount
    layoutBindings // pBindings
  };

  auto result = m_deviceFuncs->vkCreateDescriptorSetLayout(
    m_device,
    &descLayoutInfo,
    nullptr,
    &_material->descSetLayout
  );

  if (result != VK_SUCCESS)
  {
    qFatal("Failed to create descriptor set layout: %d", result);
  }
}

void PipelineHelper::allocateDescriptorSets(
  const MaterialPtr &_material
)
{
  descriptor::AllocInfo descSetAllocInfo = {
    VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
    nullptr,
    _material->descPool,
    1,
    &_material->descSetLayout
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
}

/**
 * per Material Pipeline
 */
void PipelineHelper::createDescriptorSets(
  const MaterialPtr &_material
)
{
  createDescriptorPool(_material);
  createDescriptorSetLayout(_material);
  allocateDescriptorSets(_material);
}
