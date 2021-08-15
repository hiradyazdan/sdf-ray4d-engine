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
  m_sdfUniformStartOffset = setDynamicAlignmentOffset(
    0 + m_actorVertexMemReq.size,
    m_sdfUniformMemReq.alignment
  );
  m_actorMaterial->uniMemStartOffset = setDynamicAlignmentOffset(
    m_sdfUniformStartOffset + m_sdfUniformMemReq.size,
    m_dynamicUniformMemReq.alignment
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
  descriptor::Write descWrite[3] = {}; // memset

  auto actorVertLayoutBindings = m_actorMaterial->layoutBindings[0];
  auto actorFragLayoutBindings = m_actorMaterial->layoutBindings[1];

  // Write descriptors for the dynamic uniform buffer in the vertex and fragment shaders.
  descriptor::BufferInfo vertUni = {
    m_dynamicUniformBuffer, // buffer
    0, // offset
    m_actorMaterial->vertUniSize // range
  };
  descriptor::BufferInfo fragUni = {
    m_dynamicUniformBuffer, // buffer
    m_actorMaterial->vertUniSize, // offset
    m_actorMaterial->fragUniSize // range
  };

  descWrite[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
  descWrite[0].dstSet = m_actorMaterial->descSet;
  descWrite[0].dstBinding = actorVertLayoutBindings.binding;
  descWrite[0].descriptorCount = actorVertLayoutBindings.descriptorCount;
  descWrite[0].descriptorType = actorVertLayoutBindings.descriptorType;
  descWrite[0].pBufferInfo = &vertUni;

  descWrite[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
  descWrite[1].dstSet = m_actorMaterial->descSet;
  descWrite[1].dstBinding = actorFragLayoutBindings.binding;
  descWrite[1].descriptorCount = actorFragLayoutBindings.descriptorCount;
  descWrite[1].descriptorType = actorFragLayoutBindings.descriptorType;
  descWrite[1].pBufferInfo = &fragUni;

//  auto sdfrVertLayoutBindings = m_sdfrMaterial->layoutBindings[0];
  auto sdfrFragLayoutBindings = m_sdfrMaterial->layoutBindings[0];

//  descriptor::BufferInfo sdfrVertUni = {
//    m_sdfUniformBuffer, // buffer
//    0, // offset
//    m_sdfrMaterial->vertUniSize // range
//  };

  createDepthImageView();

  descriptor::ImageInfo imageInfo = {};
  imageInfo.imageLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL;
  imageInfo.imageView = m_imageView;
  imageInfo.sampler = m_sdfrMaterial->depthTexture.getSampler();

//  descWrite[2].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
//  descWrite[2].dstSet = m_sdfrMaterial->descSets[0];
//  descWrite[2].dstBinding = sdfrVertLayoutBindings.binding;
//  descWrite[2].descriptorCount = sdfrVertLayoutBindings.descriptorCount;
//  descWrite[2].descriptorType = sdfrVertLayoutBindings.descriptorType;
//  descWrite[2].pBufferInfo = &sdfrVertUni;

  descWrite[2].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
  descWrite[2].dstSet = m_sdfrMaterial->descSet;
  descWrite[2].dstBinding = sdfrFragLayoutBindings.binding;
  descWrite[2].descriptorCount = sdfrFragLayoutBindings.descriptorCount;
  descWrite[2].descriptorType = sdfrFragLayoutBindings.descriptorType;
  descWrite[2].pImageInfo = &imageInfo;

  m_deviceFuncs->vkUpdateDescriptorSets(
    m_device,
    3,
    descWrite,
    0,
    nullptr
  );
}

void Renderer::createBuffers()
{
  if (m_actorVertexBuffer)
  {
    return;
  }

  markViewProjDirty();

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
   * Uniform buffer. Instead of using multiple descriptor sets, we take a
   * different approach: have a single dynamic uniform buffer and specify the
   * active-frame-specific offset at the time of binding the descriptor set.
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

  bindBufferToMemory(m_actorVertexBuffer, 0);
  bindBufferToMemory(m_sdfUniformBuffer, m_sdfUniformStartOffset);
  bindBufferToMemory(m_dynamicUniformBuffer, m_actorMaterial->uniMemStartOffset);

  mapMemory(actorBufferSize);
  updateDescriptorSets();
}
