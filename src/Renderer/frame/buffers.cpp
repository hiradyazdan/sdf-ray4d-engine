/*****************************************************
 * Partial Class: Renderer
 * Members: Frame - Buffers (Private)
 *****************************************************/

#include "Renderer.hpp"

using namespace sdfRay4d;

/**
 *
 * @param[in] _size
 * @param[in] _usage
 * @param[in, out] _buffer
 * @param[in, out] _memReq
 */
void Renderer::createBuffer(
  device::Size _size,
  buffer::UsageFlags _usage,
  Buffer &_buffer,
  memory::Reqs &_memReq
)
{
  buffer::Info bufInfo = {}; // memset
  bufInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
  bufInfo.size = _size;
  bufInfo.usage = _usage;

  auto result = m_deviceFuncs->vkCreateBuffer(
    m_device,
    &bufInfo,
    nullptr,
    &_buffer
  );

  if (result != VK_SUCCESS)
  {
    qFatal("Failed to create buffer: %d", result);
  }

  m_buffers.push_back(_buffer);

  m_deviceFuncs->vkGetBufferMemoryRequirements(
    m_device,
    _buffer,
    &_memReq
  );
}

void Renderer::allocateMemory()
{
  // Allocate memory for everything at once.
  m_sdfUniformStartOffset = setDynamicOffsetAlignment(
    0 + m_actorVertexMemReq.size
  );
  m_actorMaterial->uniMemStartOffset = setDynamicOffsetAlignment(
    m_sdfUniformStartOffset + m_sdfUniformMemReq.size
  );

  memory::AllocInfo memAllocInfo = {
    VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
    nullptr,
    m_actorMaterial->uniMemStartOffset + m_dynamicUniformMemReq.size,
    m_vkWindow->hostVisibleMemoryIndex()
  };
  auto result = m_deviceFuncs->vkAllocateMemory(
    m_device,
    &memAllocInfo,
    nullptr,
    &m_bufferMemory
  );

  if (result != VK_SUCCESS)
  {
    qFatal("Failed to allocate memory: %d", result);
  }
}

void Renderer::bindBufferToMemory(
  const Buffer &_buffer,
  const device::Size &_memOffset
)
{
  auto result = m_deviceFuncs->vkBindBufferMemory(
    m_device,
    _buffer,
    m_bufferMemory,
    _memOffset
  );

  if (result != VK_SUCCESS)
  {
    qFatal("Failed to bind buffer memory: %d", result);
  }
}

void Renderer::mapMemory(size_t _byteSize)
{
  // Copy vertex data.
  quint8 *p;
  auto result = m_deviceFuncs->vkMapMemory(
    m_device,
    m_bufferMemory,
    0,
    VK_WHOLE_SIZE/*m_actorMaterial->uniMemStartOffset*/,
    0,
    reinterpret_cast<void**>(&p)
  );

  if (result != VK_SUCCESS)
  {
    qFatal("Failed to map memory: %d", result);
  }

  memcpy(p, m_actorMesh.data()->geom.constData(), _byteSize);
  //  memcpy(p + m_sdfUniformStartOffset, m_sdfUniformBuffer, );
  m_deviceFuncs->vkUnmapMemory(m_device, m_bufferMemory);
}

void Renderer::updateDescriptorSets()
{
  const auto descWriteCount = 4;
  descriptor::Write descWrite[descWriteCount] = {}; // memset

  auto depthVertLayoutBindings = m_depthMaterial->layoutBindings[0];

  // Write descriptors for the dynamic uniform buffer in the vertex shaders.
  descriptor::BufferInfo depthVertexBuffer = {
    m_depthVertexBuffer, // buffer
    0, // offset
    m_depthMaterial->vertUniSize // range
  };

  descriptor::ImageInfo imageInfo = {}; // memset
  imageInfo.imageLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL;
  imageInfo.imageView = m_depthMaterial->texture.getImageView();
  imageInfo.sampler = m_depthMaterial->texture.getSampler();

  descWrite[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
  descWrite[0].dstSet = m_depthMaterial->descSets[0];
  descWrite[0].dstBinding = depthVertLayoutBindings.binding;
  descWrite[0].descriptorCount = depthVertLayoutBindings.descriptorCount;
  descWrite[0].descriptorType = depthVertLayoutBindings.descriptorType;
  descWrite[0].pBufferInfo = &depthVertexBuffer;

  auto actorVertLayoutBindings = m_actorMaterial->layoutBindings[0];
  auto actorFragLayoutBindings = m_actorMaterial->layoutBindings[1];

  // Descriptors for the dynamic uniform buffer in the vertex and fragment shaders.
  descriptor::BufferInfo actorVertUni = {
    m_dynamicUniformBuffer, // buffer
    0, // offset
    m_actorMaterial->vertUniSize // range
  };
  descriptor::BufferInfo actorFragUni = {
    m_dynamicUniformBuffer, // buffer
    m_actorMaterial->vertUniSize, // offset
    m_actorMaterial->fragUniSize // range
  };

  descWrite[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
  descWrite[1].dstSet = m_actorMaterial->descSets[0];
  descWrite[1].dstBinding = actorVertLayoutBindings.binding;
  descWrite[1].descriptorCount = actorVertLayoutBindings.descriptorCount;
  descWrite[1].descriptorType = actorVertLayoutBindings.descriptorType;
  descWrite[1].pBufferInfo = &actorVertUni;

  descWrite[2].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
  descWrite[2].dstSet = m_actorMaterial->descSets[0];
  descWrite[2].dstBinding = actorFragLayoutBindings.binding;
  descWrite[2].descriptorCount = actorFragLayoutBindings.descriptorCount;
  descWrite[2].descriptorType = actorFragLayoutBindings.descriptorType;
  descWrite[2].pBufferInfo = &actorFragUni;

  auto sdfrFragLayoutBindings = m_sdfrMaterial->layoutBindings[0];

  descWrite[3].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
  descWrite[3].dstSet = m_sdfrMaterial->descSets[0];
  descWrite[3].dstBinding = sdfrFragLayoutBindings.binding;
  descWrite[3].descriptorCount = sdfrFragLayoutBindings.descriptorCount;
  descWrite[3].descriptorType = sdfrFragLayoutBindings.descriptorType;
  descWrite[3].pImageInfo = &imageInfo;

  m_deviceFuncs->vkUpdateDescriptorSets(
    m_device,
    descWriteCount,
    descWrite,
    0,
    nullptr
  );
}

void Renderer::createBuffers()
{
  if (m_depthVertexBuffer)
  {
    return;
  }

  markViewProjDirty();

  const auto depthBufferSize = m_actorMesh.data()->vertexCount * 8 * sizeof(float); // TODO: temp

  /**
   * Depth Vertex Buffer
   */
  createBuffer(
    depthBufferSize,
    VK_BUFFER_USAGE_VERTEX_BUFFER_BIT |
    VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
    m_depthVertexBuffer,
    m_depthVertexMemReq
  );

  const auto actorBufferSize = m_actorMesh.data()->vertexCount * 8 * sizeof(float);

  /**
   * Actor Vertex Buffer
   */
  createBuffer(
    actorBufferSize,
    VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
    m_actorVertexBuffer,
    m_actorVertexMemReq
  );

  auto Kb = (1 << 10);
  //  auto FRAME_COUNT = 2;
  auto UPLOAD_REGION_SIZE = 64 * Kb;
  auto UPLOAD_BUFFER_SIZE = m_vpDirty * UPLOAD_REGION_SIZE;

  /**
   * Raymarch (SDF) Fragment Buffer
   */
  createBuffer(
    UPLOAD_BUFFER_SIZE,
    VK_BUFFER_USAGE_VERTEX_BUFFER_BIT  |
    VK_BUFFER_USAGE_INDEX_BUFFER_BIT   |
    VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT |
    VK_BUFFER_USAGE_STORAGE_BUFFER_BIT |
    VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
    m_sdfUniformBuffer,
    m_sdfUniformMemReq
  );

  /**
   * Actor Uniform Buffer
   *
   * @note
   *
   * Instead of using multiple descriptor sets, a single dynamic uniform buffer is used
   * and active frame offset is set at the time of binding the descriptor set.
   */
  createBuffer(
    (
      m_actorMaterial->vertUniSize +
      m_actorMaterial->fragUniSize
    ) * m_vpDirty,
    VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
    m_dynamicUniformBuffer,
    m_dynamicUniformMemReq
  );

  allocateMemory();

  bindBufferToMemory(m_depthVertexBuffer, 0);
  bindBufferToMemory(m_actorVertexBuffer, m_depthMaterial->vertUniSize);
  bindBufferToMemory(m_sdfUniformBuffer, m_sdfUniformStartOffset);
  bindBufferToMemory(m_dynamicUniformBuffer, m_actorMaterial->uniMemStartOffset);

  mapMemory(actorBufferSize);
  updateDescriptorSets();
}
