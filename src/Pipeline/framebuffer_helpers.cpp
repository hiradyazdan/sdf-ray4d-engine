/*****************************************************
 * Partial Class: Pipeline
 * Members: Framebuffer Helpers (Public/Private)
 *****************************************************/

#include "Pipeline.hpp"

using namespace sdfRay4d;

Framebuffer &PipelineHelper::getFramebuffer(uint32_t _width, uint32_t _height)
{
  /**
   * @note
   * Since this method is called per frame,
   * to make framebuffer instance reusable/non-copyable
   * and so to be able to draw to it per frame,
   * like a singleton instance
   */
  if(!m_frameBuffer)
  {
    createFramebuffer(_width, _height);
  }

  return m_frameBuffer;
}

void PipelineHelper::createFramebuffer(uint32_t _width, uint32_t _height)
{
  FramebufferInfo framebufferInfo = {};
  framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
  framebufferInfo.renderPass = getRenderPass(false);
  framebufferInfo.attachmentCount = 1; // depth pass attachment image view is only 1
  framebufferInfo.pAttachments = m_attachments;
  framebufferInfo.width = _width;
  framebufferInfo.height = _height;
  framebufferInfo.layers = 1;

  auto result = m_deviceFuncs->vkCreateFramebuffer(
    m_device,
    &framebufferInfo,
    nullptr,
    &m_frameBuffer
  );

  if (result != VK_SUCCESS)
  {
    qWarning("Failed to create RenderPass: %d", result);
  }
}
