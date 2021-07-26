/*****************************************************
 * Partial Class: Renderer
 * Members: Init Resources Helpers (Private)
 *****************************************************/

#include "Renderer.hpp"

using namespace sdfRay4d;

//void Renderer::createBuffer()
//{
//  BufferInfo bufferInfo;
//  VkMemoryRequirements memoryRequirements;
//
//  memset(&bufferInfo, 0, sizeof(bufferInfo));
//  bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
//  const int blockMeshByteCount = m_blockMesh.data()->vertexCount * 8 * sizeof(float);
//  bufferInfo.size = blockMeshByteCount;
//  bufferInfo.usage =
//    VK_BUFFER_USAGE_VERTEX_BUFFER_BIT |
//    VK_BUFFER_USAGE_INDEX_BUFFER_BIT |
//    VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT |
//    VK_BUFFER_USAGE_STORAGE_BUFFER_BIT |
//    VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
//
//  auto result = m_deviceFuncs->vkCreateBuffer(m_device, &bufferInfo, nullptr, &m_buf);
//  if (result != VK_SUCCESS)
//  {
//    qFatal("Failed to create buffer: %d", result);
//  }
//
//  m_deviceFuncs->vkGetBufferMemoryRequirements(m_device, m_buf, &memoryRequirements);
//}
//
//void Renderer::allocateMemory()
//{
////  // Allocate memory for everything at once.
////  auto logoVertStartOffset = aligned(0 + blockVertMemReq.size, logoVertMemReq.alignment);
////  auto floorVertStartOffset = aligned(logoVertStartOffset + logoVertMemReq.size, floorVertMemReq.alignment);
////  m_itemMaterial.uniMemStartOffset = aligned(floorVertStartOffset + floorVertMemReq.size, uniMemReq.alignment);
//
//  memory::AllocInfo allocInfo;
//  allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
//  allocInfo.pNext = nullptr;
//  allocInfo.allocationSize = memoryRequirements.size;
//  allocInfo.memoryTypeIndex = m_vkWindow->hostVisibleMemoryIndex();
//
//  auto result = m_deviceFuncs->vkAllocateMemory(m_device, &allocInfo, nullptr, &m_bufMem);
//  if (result != VK_SUCCESS)
//  {
//    qFatal("Failed to allocate memory: %d", result);
//  }
//}
//
//void Renderer::mapBufferMemory()
//{
//  auto result = m_deviceFuncs->vkBindBufferMemory(m_device, m_buf, m_bufMem, 0);
//  if (result != VK_SUCCESS)
//  {
//    qFatal("Failed to bind buffer memory: %d", result);
//  }
//
//  quint8 *bufferPtr;
//  result = m_deviceFuncs->vkMapMemory(
//    m_device,
//    m_bufMem,
//    0,
//    VK_WHOLE_SIZE, // m_objMaterial.uniMemStartOffset
//    0,
//    reinterpret_cast<void **>(&bufferPtr)
//  );
//  if (result != VK_SUCCESS)
//  {
//    qFatal("Failed to map memory: %d", result);
//  }
//}
//
//void Renderer::createUploadBuffer(int _concurrentFrameCount)
//{
//  createBuffer();
//  allocateMemory();
//  mapBufferMemory();
//
//  bufferCreateInfo = (VkBufferCreateInfo){
//    .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
//    .size = STATIC_BUFFER_SIZE,
//    .usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT
//             | VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT
//             | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
//    .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
//    .queueFamilyIndexCount = 1,
//    .pQueueFamilyIndices = &queueFamilyIndex,
//  };
//}

/**
 * per Material Pipeline
 */
void Renderer::setShaderStages()
{
  pipeline::ShaderStageInfo shaderStages[2] = {
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
      VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
      nullptr,
      0,
      VK_SHADER_STAGE_FRAGMENT_BIT,
      m_objMaterial.fragmentShader.getData()->shaderModule,
      "main",
      nullptr
    }
  };

  memcpy(&m_shaderStages, &shaderStages, sizeof(shaderStages));
}

/**
 * per Material Pipeline
 */
void Renderer::setVertexLayout()
{
  VertexBindingDesc vertexBindingDesc = {
    0, // binding
    3 * sizeof(float), // sizeof(VertexP2C)
    VK_VERTEX_INPUT_RATE_VERTEX
  };
  VertexAttrDesc vertexAttrDesc[] = {
    { // position
      0, // location
      0, // binding
      VK_FORMAT_R32G32_SFLOAT,
      0 // offsetof(VertexP2C, x)
    },
    { // color
      1,
      0,
      VK_FORMAT_R32G32B32_SFLOAT, // VK_FORMAT_R8G8B8A8_UNORM
      2 * sizeof(float) // offsetof(VertexP2C, rgba)
    }
  };

  pipeline::VertexInputInfo vertexInputInfo;
  vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
  vertexInputInfo.vertexBindingDescriptionCount = sizeof(vertexBindingDesc);
  vertexInputInfo.pVertexBindingDescriptions = &vertexBindingDesc;
  vertexInputInfo.vertexAttributeDescriptionCount = sizeof(vertexAttrDesc) / sizeof(vertexAttrDesc[0]);
  vertexInputInfo.pVertexAttributeDescriptions = vertexAttrDesc;
  vertexInputInfo.pNext = nullptr;
  vertexInputInfo.flags = 0;

  memcpy(&m_vertexInputInfo, &vertexInputInfo, sizeof(vertexInputInfo));
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
