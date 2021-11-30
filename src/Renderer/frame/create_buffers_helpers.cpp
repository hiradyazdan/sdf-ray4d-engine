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

  auto &buffer = m_pipelineHelper.getBufferHelper();

  for (const auto &material : m_materials)
  {
    if(material == m_sdfrMaterial)
    {
      material->bufferSize *= m_concurrentFrameCount;
    }

    buffer.createBuffer(
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
  buffer.createBuffer(
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

  buffer.allocateMemory(
    m_actorMaterial->uniMemStartOffset + m_actorMaterial->dynamicUniformMemReq.size,
    m_vkWindow->hostVisibleMemoryIndex()
  );

  buffer.bindBufferMemory(m_depthMaterial->buffer, 0);
  buffer.bindBufferMemory(m_actorMaterial->buffer, m_depthMaterial->vertUniSize);
  buffer.bindBufferMemory(m_sdfrMaterial->buffer, sdfUniformStartOffset);

  buffer.bindBufferMemory(m_actorMaterial->dynamicUniformBuffer, m_actorMaterial->uniMemStartOffset);

  updateDescriptorSets();
}

void Renderer::updateDescriptorSets()
{
  auto &descriptor = m_pipelineHelper.getDescriptorHelper();

  descriptor.addWriteSet(
    m_depthMaterial->descSets[0],
    m_depthMaterial->layoutBindings[0],
    {
      m_depthMaterial->buffer, // buffer
      0, // offset
      m_depthMaterial->vertUniSize // range
    }
  );
  // Descriptors for the dynamic uniform buffer in the vertex and fragment shaders
  descriptor.addWriteSet(
    m_actorMaterial->descSets[0],
    m_actorMaterial->layoutBindings[0],
    {
      m_actorMaterial->dynamicUniformBuffer, // buffer
      0, // offset
      m_actorMaterial->vertUniSize // range
    }
  );
  descriptor.addWriteSet(
    m_actorMaterial->descSets[0],
    m_actorMaterial->layoutBindings[1],
    {
      m_actorMaterial->dynamicUniformBuffer, // buffer
      m_actorMaterial->vertUniSize, // offset
      m_actorMaterial->fragUniSize // range
    }
  );
  descriptor.addWriteSet(
    m_sdfrMaterial->descSets[0],
    m_sdfrMaterial->layoutBindings[0],
    {
      m_sdfrMaterial->texture.getSampler(), // sampler
      m_depthMaterial->texture.getImageView(), // imageView
      VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL, // imageLayout
    }
  );

  descriptor.updateDescriptorSets();
}
