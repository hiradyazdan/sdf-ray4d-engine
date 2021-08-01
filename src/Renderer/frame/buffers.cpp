/*****************************************************
 * Partial Class: Renderer
 * Members: Frame - Buffers (Private)
 *****************************************************/

#include "Renderer.hpp"

using namespace sdfRay4d;

void Renderer::createBuffers()
{
  if (m_uniformBuffer) return;

  markViewProjDirty();

  auto Kb = (1 << 10);
  auto FRAME_COUNT = 2;
  auto UPLOAD_REGION_SIZE = 64 * Kb;
  auto UPLOAD_BUFFER_SIZE = m_vpDirty * UPLOAD_REGION_SIZE;

  BufferInfo bufferInfo = {}; // memset
  bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
  bufferInfo.size  = UPLOAD_BUFFER_SIZE;//(m_objMaterial.vertUniSize + m_objMaterial.fragUniSize) * concurrentFrameCount;
  bufferInfo.usage =
    VK_BUFFER_USAGE_VERTEX_BUFFER_BIT  |
    VK_BUFFER_USAGE_INDEX_BUFFER_BIT   |
    VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT |
    VK_BUFFER_USAGE_STORAGE_BUFFER_BIT |
    VK_BUFFER_USAGE_TRANSFER_SRC_BIT;

  auto result = m_deviceFuncs->vkCreateBuffer(
    m_device,
    &bufferInfo,
    nullptr,
    &m_uniformBuffer
  );

  if (result != VK_SUCCESS)
  {
    qFatal("Failed to create uniform buffer: %d", result);
  }

  memory::Reqs uniMemReq;
  m_deviceFuncs->vkGetBufferMemoryRequirements(m_device, m_uniformBuffer, &uniMemReq);

  // Allocate memory for everything at once.
//  m_objMaterial.uniMemStartOffset = aligned(floorVertMemReq.alignment + floorVertMemReq.size, uniMemReq.alignment);
  memory::AllocInfo memAllocInfo = {
    VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
    nullptr,
    /*m_objMaterial.uniMemStartOffset + */uniMemReq.size,
    m_vkWindow->hostVisibleMemoryIndex()
  };
  result = m_deviceFuncs->vkAllocateMemory(m_device, &memAllocInfo, nullptr, &m_bufMem);

  if (result != VK_SUCCESS)
  {
    qFatal("Failed to allocate memory: %d", result);
  }

  result = m_deviceFuncs->vkBindBufferMemory(
    m_device,
    m_uniformBuffer,
    m_bufMem,
    0/*m_objMaterial.uniMemStartOffset*/
  );

  if (result != VK_SUCCESS)
  {
    qFatal("Failed to bind uniform buffer memory: %d", result);
  }

  // Copy vertex data.
  quint8 *p;
  result = m_deviceFuncs->vkMapMemory(
    m_device,
    m_bufMem,
    0,
    VK_WHOLE_SIZE/*m_objMaterial.uniMemStartOffset*/,
    0,
    reinterpret_cast<void**>(&p)
  );

  if (result != VK_SUCCESS)
  {
    qFatal("Failed to map memory: %d", result);
  }
}
