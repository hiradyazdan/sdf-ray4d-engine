/*****************************************************
 * Partial Class: Renderer
 * Members: Init Resources Helpers (Private)
 *****************************************************/

#include "Renderer.hpp"

using namespace sdfRay4d;

/**
 * per Material Pipeline
 */
void Renderer::initShaderStages()
{
  m_objMaterial.shaderStages = {
    {
      VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO, // sType
      nullptr, // pNext
      0, // flags
      VK_SHADER_STAGE_VERTEX_BIT, // stage
      m_objMaterial.vertexShader.getData()->shaderModule, // module
      "main", // pName
      nullptr // pSpecializationInfo
    },
    {
      VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO, // sType
      nullptr, // pNext
      0, // flags
      VK_SHADER_STAGE_FRAGMENT_BIT, // stage
      m_objMaterial.fragmentShader.getData()->shaderModule, // module
      "main", // pName
      nullptr // pSpecializationInfo
    }
  };
}

/**
 * per Material Pipeline
 */
void Renderer::setupDescriptorSets()
{
  // Descriptor set layout.
  VkDescriptorPoolSize descPoolSizes[] = {
    { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 2 }
  };
  VkDescriptorPoolCreateInfo descPoolInfo;
  memset(&descPoolInfo, 0, sizeof(descPoolInfo));
  descPoolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
  descPoolInfo.maxSets = 1; // a single set is enough due to the dynamic uniform buffer
  descPoolInfo.poolSizeCount = sizeof(descPoolSizes) / sizeof(descPoolSizes[0]);
  descPoolInfo.pPoolSizes = descPoolSizes;
  VkResult err = m_deviceFuncs->vkCreateDescriptorPool(m_device, &descPoolInfo, nullptr, &m_objMaterial.descPool);
  if (err != VK_SUCCESS)
    qFatal("Failed to create descriptor pool: %d", err);

  VkDescriptorSetLayoutBinding layoutBindings[] =
    {
      {
        0, // binding
        VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC,
        1, // descriptorCount
        VK_SHADER_STAGE_VERTEX_BIT,
        nullptr
      },
      {
        1,
        VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC,
        1,
        VK_SHADER_STAGE_FRAGMENT_BIT,
        nullptr
      }
    };
  VkDescriptorSetLayoutCreateInfo descLayoutInfo = {
    VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
    nullptr,
    0,
    sizeof(layoutBindings) / sizeof(layoutBindings[0]),
    layoutBindings
  };
  err = m_deviceFuncs->vkCreateDescriptorSetLayout(
    m_device,
    &descLayoutInfo,
    nullptr,
    &m_objMaterial.descSetLayout
  );
  if (err != VK_SUCCESS)
    qFatal("Failed to create descriptor set layout: %d", err);

  VkDescriptorSetAllocateInfo descSetAllocInfo = {
    VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
    nullptr,
    m_objMaterial.descPool,
    1,
    &m_objMaterial.descSetLayout
  };
  err = m_deviceFuncs->vkAllocateDescriptorSets(m_device, &descSetAllocInfo, &m_objMaterial.descSet);
  if (err != VK_SUCCESS)
    qFatal("Failed to allocate descriptor set: %d", err);
}
