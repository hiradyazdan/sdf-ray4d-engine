/*****************************************************
 * Partial Class: Renderer
 * Members: Init Resources Helpers (Private)
 *****************************************************/

#include "Renderer.hpp"

using namespace sdfRay4d;
using namespace vk;

void Renderer::createUploadBuffer(int _concurrentFrameCount)
{
  // Prepare the vertex and uniform data. The vertex data will never
  // change so one buffer is sufficient regardless of the value of
  // QVulkanWindow::CONCURRENT_FRAME_COUNT. Uniform data is changing per
  // frame however so active frames have to have a dedicated copy.

  // Use just one memory allocation and one buffer. We will then specify the
  // appropriate offsets for uniform buffers in the VkDescriptorBufferInfo.
  // Have to watch out for
  // VkPhysicalDeviceLimits::minUniformBufferOffsetAlignment, though.

  // The uniform buffer is not strictly required in this example, we could
  // have used push constants as well since our single matrix (64 bytes) fits
  // into the spec mandated minimum limit of 128 bytes. However, once that
  // limit is not sufficient, the per-frame buffers, as shown below, will
  // become necessary.

  const PhysicalDeviceLimits *pdevLimits = &m_vkWindow->physicalDeviceProperties()->limits;
  const device::Size uniAlign = pdevLimits->minUniformBufferOffsetAlignment;
  qDebug("uniform buffer offset alignment is %u", (uint) uniAlign);
  BufferInfo bufInfo;
  memset(&bufInfo, 0, sizeof(bufInfo));
  bufInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
  // Our internal layout is vertex, uniform, uniform, ... with each uniform buffer start offset aligned to uniAlign.
  const device::Size vertexAllocSize = aligned(sizeof(vertexData), uniAlign);
  const device::Size uniformAllocSize = aligned(UNIFORM_DATA_SIZE, uniAlign);
  bufInfo.size = vertexAllocSize + _concurrentFrameCount * uniformAllocSize;
  bufInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;

  Result result = m_deviceFuncs->vkCreateBuffer(m_device, &bufInfo, nullptr, &m_buf);
  if (result != VK_SUCCESS)
    qFatal("Failed to create buffer: %d", result);

  MemReq memReq;
  m_deviceFuncs->vkGetBufferMemoryRequirements(m_device, m_buf, &memReq);

  MemAlloc memAllocInfo = {
    VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
    nullptr,
    memReq.size,
    m_vkWindow->hostVisibleMemoryIndex()
  };

  result = m_deviceFuncs->vkAllocateMemory(m_device, &memAllocInfo, nullptr, &m_bufMem);
  if (result != VK_SUCCESS)
    qFatal("Failed to allocate memory: %d", result);

  result = m_deviceFuncs->vkBindBufferMemory(m_device, m_buf, m_bufMem, 0);
  if (result != VK_SUCCESS)
    qFatal("Failed to bind buffer memory: %d", result);

  quint8 *p;
  result = m_deviceFuncs->vkMapMemory(m_device, m_bufMem, 0, memReq.size, 0, reinterpret_cast<void **>(&p));
  if (result != VK_SUCCESS)
    qFatal("Failed to map memory: %d", result);
  memcpy(p, vertexData, sizeof(vertexData));
  QMatrix4x4 ident;
  memset(m_uniformBufInfo, 0, sizeof(m_uniformBufInfo));
  for (int i = 0; i < _concurrentFrameCount; ++i) {
    const device::Size offset = vertexAllocSize + i * uniformAllocSize;
    memcpy(p + offset, ident.constData(), 16 * sizeof(float));
    m_uniformBufInfo[i].buffer = m_buf;
    m_uniformBufInfo[i].offset = offset;
    m_uniformBufInfo[i].range = uniformAllocSize;
  }
  m_deviceFuncs->vkUnmapMemory(m_device, m_bufMem);
}

void Renderer::setupDescriptorSets(int _concurrentFrameCount)
{
  // Set up descriptor set and its layout.
  descriptor::PoolSize descPoolSizes = { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, uint32_t(_concurrentFrameCount) };
  descriptor::PoolInfo descPoolInfo;
  memset(&descPoolInfo, 0, sizeof(descPoolInfo));
  descPoolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
  descPoolInfo.maxSets = _concurrentFrameCount;
  descPoolInfo.poolSizeCount = 1;
  descPoolInfo.pPoolSizes = &descPoolSizes;
  Result result = m_deviceFuncs->vkCreateDescriptorPool(m_device, &descPoolInfo, nullptr, &m_descPool);
  if (result != VK_SUCCESS)
    qFatal("Failed to create descriptor pool: %d", result);

  descriptor::LayoutBinding layoutBinding = {
    0, // binding
    VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
    1,
    VK_SHADER_STAGE_VERTEX_BIT,
    nullptr
  };
  descriptor::LayoutInfo descLayoutInfo = {
    VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
    nullptr,
    0,
    1,
    &layoutBinding
  };
  result = m_deviceFuncs->vkCreateDescriptorSetLayout(m_device, &descLayoutInfo, nullptr, &m_descSetLayout);
  if (result != VK_SUCCESS)
    qFatal("Failed to create descriptor set layout: %d", result);

  for (int i = 0; i < _concurrentFrameCount; ++i) {
    descriptor::AllocInfo descSetAllocInfo = {
      VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
      nullptr,
      m_descPool,
      1,
      &m_descSetLayout
    };
    result = m_deviceFuncs->vkAllocateDescriptorSets(m_device, &descSetAllocInfo, &m_descSet[i]);
    if (result != VK_SUCCESS)
      qFatal("Failed to allocate descriptor set: %d", result);

    descriptor::Write descWrite;
    memset(&descWrite, 0, sizeof(descWrite));
    descWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descWrite.dstSet = m_descSet[i];
    descWrite.descriptorCount = 1;
    descWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    descWrite.pBufferInfo = &m_uniformBufInfo[i];
    m_deviceFuncs->vkUpdateDescriptorSets(m_device, 1, &descWrite, 0, nullptr);
  }
}
