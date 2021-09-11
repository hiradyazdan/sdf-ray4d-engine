/*****************************************************
 * Partial Class: Renderer
 * Members: Frame - Buffers (Private)
 *****************************************************/

#include "Renderer.hpp"

using namespace sdfRay4d;

void Renderer::createBuffers()
{
  if (m_depthMaterial->buffer) return;

  markViewProjDirty();

  for (const auto &material : m_materials)
  {
    if(material == m_sdfrMaterial)
    {
      material->bufferSize *= m_concurrentFrameCount;
    }

    m_pipelineHelper.buffer.createBuffer(
      material->bufferSize,
      material->bufferUsage,
      material->buffer,
      material->memReq
    );
  }

  /**
   * Actor Dynamic Uniform Buffer
   *
   * @note
   *
   * Instead of using multiple descriptor sets, a single dynamic uniform buffer is used
   * and active frame offset is set at the time of binding the descriptor set.
   */
  m_pipelineHelper.buffer.createBuffer(
    (
      m_actorMaterial->vertUniSize +
      m_actorMaterial->fragUniSize
    ) * m_concurrentFrameCount,
    VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
    m_actorMaterial->dynamicUniformBuffer,
    m_actorMaterial->dynamicUniformMemReq
  );

  // Allocate memory for everything at once.
  device::Size sdfUniformStartOffset = setDynamicOffsetAlignment(
    0 + m_actorMaterial->memReq.size
  );
  m_actorMaterial->uniMemStartOffset = setDynamicOffsetAlignment(
    sdfUniformStartOffset + m_sdfrMaterial->memReq.size
  );

  m_pipelineHelper.buffer.allocateMemory(
    m_actorMaterial->uniMemStartOffset + m_actorMaterial->dynamicUniformMemReq.size,
    m_vkWindow->hostVisibleMemoryIndex()
  );

  m_pipelineHelper.buffer.bindBufferMemory(m_depthMaterial->buffer, 0);
  m_pipelineHelper.buffer.bindBufferMemory(m_actorMaterial->buffer, m_depthMaterial->vertUniSize);
  m_pipelineHelper.buffer.bindBufferMemory(m_sdfrMaterial->buffer, sdfUniformStartOffset);

  m_pipelineHelper.buffer.bindBufferMemory(m_actorMaterial->dynamicUniformBuffer, m_actorMaterial->uniMemStartOffset);

  updateDescriptorSets();
}

void Renderer::updateDescriptorSets()
{
  m_pipelineHelper.descriptor.addWriteSet(
    m_depthMaterial->descSets[0],
    m_depthMaterial->layoutBindings[0],
    {
      m_depthMaterial->buffer, // buffer
      0, // offset
      m_depthMaterial->vertUniSize // range
    }
  );
  // Descriptors for the dynamic uniform buffer in the vertex and fragment shaders
  m_pipelineHelper.descriptor.addWriteSet(
    m_actorMaterial->descSets[0],
    m_actorMaterial->layoutBindings[0],
    {
      m_actorMaterial->dynamicUniformBuffer, // buffer
      0, // offset
      m_actorMaterial->vertUniSize // range
    }
  );
  m_pipelineHelper.descriptor.addWriteSet(
    m_actorMaterial->descSets[0],
    m_actorMaterial->layoutBindings[1],
    {
      m_actorMaterial->dynamicUniformBuffer, // buffer
      m_actorMaterial->vertUniSize, // offset
      m_actorMaterial->fragUniSize // range
    }
  );

  descriptor::ImageInfo imageInfo = {}; // memset
  imageInfo.imageLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL;
  imageInfo.imageView = m_depthMaterial->texture.getImageView();
  imageInfo.sampler = m_sdfrMaterial->texture.getSampler();

  m_pipelineHelper.descriptor.addWriteSet(
    m_sdfrMaterial->descSets[0],
    m_sdfrMaterial->layoutBindings[0],
    imageInfo
  );

  m_pipelineHelper.descriptor.updateDescriptorSets();
}
