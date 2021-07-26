/*****************************************************
 * Partial Class: Renderer
 * Members: Frame Upload Buffers (Private)
 *****************************************************/

#include "Renderer.hpp"

using namespace sdfRay4d;

void Renderer::ensureBuffers()
{
  if (m_uniformBuffer) return;

//  BufferInfo bufferInfo = { VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO };
//  bufferInfo.size = 65536;
//  bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
//
//  vma::AllocInfo allocInfo = {};
//  allocInfo.usage = VMA_MEMORY_USAGE_GPU_ONLY;
//
//  Buffer buffer;
//  vma::Alloc allocation;
//
//  vmaCreateBuffer(
//    m_allocator,
//    &bufferInfo, &allocInfo,
//    &buffer, &allocation,
//    nullptr
//  );


//  VkDevice dev = m_vkWindow->device();
//  const int concurrentFrameCount = m_vkWindow->concurrentFrameCount();

  markViewProjDirty();

  // Vertex buffer for the block.
  VkBufferCreateInfo bufInfo;
  memset(&bufInfo, 0, sizeof(bufInfo));
  bufInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
//  const int blockMeshByteCount = m_blockMesh.data()->vertexCount * 8 * sizeof(float);
//  bufInfo.size = blockMeshByteCount;
//  bufInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
//  VkResult err = m_deviceFuncs->vkCreateBuffer(m_device, &bufInfo, nullptr, &m_uniformBuffer);
//  if (err != VK_SUCCESS)
//    qFatal("Failed to create vertex buffer: %d", err);

//  VkMemoryRequirements blockVertMemReq;
//  m_deviceFuncs->vkGetBufferMemoryRequirements(m_device, m_uniformBuffer, &blockVertMemReq);

//  // Vertex buffer for the floor.
//  bufInfo.size = sizeof(quadVert);
//  err = m_deviceFuncs->vkCreateBuffer(dev, &bufInfo, nullptr, &m_floorVertexBuf);
//  if (err != VK_SUCCESS)
//    qFatal("Failed to create vertex buffer: %d", err);
//
//  VkMemoryRequirements floorVertMemReq;
//  m_deviceFuncs->vkGetBufferMemoryRequirements(dev, m_floorVertexBuf, &floorVertMemReq);

  // Uniform buffer. Instead of using multiple descriptor sets, we take a
  // different approach: have a single dynamic uniform buffer and specify the
  // active-frame-specific offset at the time of binding the descriptor set.

  auto Kb = (1 << 10);
  auto FRAME_COUNT = 2;
  auto UPLOAD_REGION_SIZE = 64 * Kb;
  auto UPLOAD_BUFFER_SIZE = m_vpDirty * UPLOAD_REGION_SIZE;

  bufInfo.size = UPLOAD_BUFFER_SIZE;//(m_objMaterial.vertUniSize + m_objMaterial.fragUniSize) * concurrentFrameCount;
  bufInfo.usage =
    VK_BUFFER_USAGE_VERTEX_BUFFER_BIT  |
    VK_BUFFER_USAGE_INDEX_BUFFER_BIT   |
    VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT |
    VK_BUFFER_USAGE_STORAGE_BUFFER_BIT |
    VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
  auto result = m_deviceFuncs->vkCreateBuffer(m_device, &bufInfo, nullptr, &m_uniformBuffer);
  if (result != VK_SUCCESS)
    qFatal("Failed to create uniform buffer: %d", result);

  VkMemoryRequirements uniMemReq;
  m_deviceFuncs->vkGetBufferMemoryRequirements(m_device, m_uniformBuffer, &uniMemReq);

  // Allocate memory for everything at once.
//  m_objMaterial.uniMemStartOffset = aligned(floorVertMemReq.alignment + floorVertMemReq.size, uniMemReq.alignment);
  VkMemoryAllocateInfo memAllocInfo = {
    VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
    nullptr,
    /*m_objMaterial.uniMemStartOffset + */uniMemReq.size,
    m_vkWindow->hostVisibleMemoryIndex()
  };
  result = m_deviceFuncs->vkAllocateMemory(m_device, &memAllocInfo, nullptr, &m_bufMem);
  if (result != VK_SUCCESS)
    qFatal("Failed to allocate memory: %d", result);

//  err = m_deviceFuncs->vkBindBufferMemory(dev, m_blockVertexBuf, m_bufMem, 0);
//  if (err != VK_SUCCESS)
//    qFatal("Failed to bind vertex buffer memory: %d", err);
//  err = m_deviceFuncs->vkBindBufferMemory(dev, m_floorVertexBuf, m_bufMem, floorVertStartOffset);
//  if (err != VK_SUCCESS)
//    qFatal("Failed to bind vertex buffer memory: %d", err);
  result = m_deviceFuncs->vkBindBufferMemory(m_device, m_uniformBuffer, m_bufMem, 0/*m_objMaterial.uniMemStartOffset*/);
  if (result != VK_SUCCESS)
    qFatal("Failed to bind uniform buffer memory: %d", result);

  // Copy vertex data.
  quint8 *p;
  result = m_deviceFuncs->vkMapMemory(m_device, m_bufMem, 0, VK_WHOLE_SIZE/*m_objMaterial.uniMemStartOffset*/, 0, reinterpret_cast<void **>(&p));
  if (result != VK_SUCCESS)
    qFatal("Failed to map memory: %d", result);

//  memcpy(p, m_blockMesh.data()->geom.constData(), blockMeshByteCount);
//  memcpy(p + logoVertStartOffset, m_logoMesh.data()->geom.constData(), logoMeshByteCount);
//  memcpy(p + floorVertStartOffset, quadVert, sizeof(quadVert));

//  m_deviceFuncs->vkUnmapMemory(m_device, m_bufMem);

  // Write descriptors for the uniform buffers in the vertex and fragment shaders.
//  VkDescriptorBufferInfo vertUni = { m_uniformBuffer, 0, m_objMaterial.vertUniSize };
//  VkDescriptorBufferInfo fragUni = { m_uniformBuffer, m_objMaterial.vertUniSize, m_objMaterial.fragUniSize };
//
//  VkWriteDescriptorSet descWrite[2];
//  memset(descWrite, 0, sizeof(descWrite));
//  descWrite[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
//  descWrite[0].dstSet = m_objMaterial.descSet;
//  descWrite[0].dstBinding = 0;
//  descWrite[0].descriptorCount = 1;
//  descWrite[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
//  descWrite[0].pBufferInfo = &vertUni;
//
//  descWrite[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
//  descWrite[1].dstSet = m_objMaterial.descSet;
//  descWrite[1].dstBinding = 1;
//  descWrite[1].descriptorCount = 1;
//  descWrite[1].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
//  descWrite[1].pBufferInfo = &fragUni;
//
//  m_deviceFuncs->vkUpdateDescriptorSets(m_device, 2, descWrite, 0, nullptr);
}

//void Renderer::ensureInstanceBuffer()
//{
//  if (m_instCount == m_preparedInstCount && m_instBuf)
//    return;
//
//  Q_ASSERT(m_instCount <= MAX_INSTANCES);
//
//  VkDevice dev = m_vkWindow->device();
//
//  // allocate only once, for the maximum instance count
//  if (!m_instBuf) {
//    VkBufferCreateInfo bufInfo;
//    memset(&bufInfo, 0, sizeof(bufInfo));
//    bufInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
//    bufInfo.size = MAX_INSTANCES * PER_INSTANCE_DATA_SIZE;
//    bufInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
//
//    // Keep a copy of the data since we may lose all graphics resources on
//    // unexpose, and reinitializing to new random positions afterwards
//    // would not be nice.
//    m_instData.resize(bufInfo.size);
//
//    VkResult err = m_deviceFuncs->vkCreateBuffer(dev, &bufInfo, nullptr, &m_instBuf);
//    if (err != VK_SUCCESS)
//      qFatal("Failed to create instance buffer: %d", err);
//
//    VkMemoryRequirements memReq;
//    m_deviceFuncs->vkGetBufferMemoryRequirements(dev, m_instBuf, &memReq);
//    if (DBG)
//      qDebug("Allocating %u bytes for instance data", uint32_t(memReq.size));
//
//    VkMemoryAllocateInfo memAllocInfo = {
//      VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
//      nullptr,
//      memReq.size,
//      m_vkWindow->hostVisibleMemoryIndex()
//    };
//    err = m_deviceFuncs->vkAllocateMemory(dev, &memAllocInfo, nullptr, &m_instBufMem);
//    if (err != VK_SUCCESS)
//      qFatal("Failed to allocate memory: %d", err);
//
//    err = m_deviceFuncs->vkBindBufferMemory(dev, m_instBuf, m_instBufMem, 0);
//    if (err != VK_SUCCESS)
//      qFatal("Failed to bind instance buffer memory: %d", err);
//  }
//
//  if (m_instCount != m_preparedInstCount) {
//    if (DBG)
//      qDebug("Preparing instances %d..%d", m_preparedInstCount, m_instCount - 1);
//    char *p = m_instData.data();
//    p += m_preparedInstCount * PER_INSTANCE_DATA_SIZE;
//    auto gen = [](int a, int b) {
//      return float(QRandomGenerator::global()->bounded(double(b - a)) + a);
//    };
//    for (int i = m_preparedInstCount; i < m_instCount; ++i) {
//      // Apply a random translation to each instance of the mesh.
//      float t[] = { gen(-5, 5), gen(-4, 6), gen(-30, 5) };
//      memcpy(p, t, 12);
//      // Apply a random adjustment to the diffuse color for each instance. (default is 0.7)
//      float d[] = { gen(-6, 3) / 10.0f, gen(-6, 3) / 10.0f, gen(-6, 3) / 10.0f };
//      memcpy(p + 12, d, 12);
//      p += PER_INSTANCE_DATA_SIZE;
//    }
//    m_preparedInstCount = m_instCount;
//  }
//
//  quint8 *p;
//  VkResult err = m_deviceFuncs->vkMapMemory(dev, m_instBufMem, 0, m_instCount * PER_INSTANCE_DATA_SIZE, 0,
//                                            reinterpret_cast<void **>(&p));
//  if (err != VK_SUCCESS)
//    qFatal("Failed to map memory: %d", err);
//  memcpy(p, m_instData.constData(), m_instData.size());
//  m_deviceFuncs->vkUnmapMemory(dev, m_instBufMem);
//}
