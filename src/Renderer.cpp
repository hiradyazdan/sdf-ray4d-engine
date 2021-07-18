//#include "GLSLCompiler.hpp"
#include "Renderer.hpp"

using namespace sdfRay4d;

Renderer::Renderer(QVulkanWindow *_vkWindow, bool _msaa) :
m_vkWindow(_vkWindow)
{
  if (_msaa)
  {
    const auto counts = _vkWindow->supportedSampleCounts();
    qDebug() << "Supported sample counts:" << counts;

    for (int s = 16; s >= 4; s /= 2)
    {
      if (counts.contains(s))
      {
        qDebug("Requesting sample count %d", s);
        m_vkWindow->setSampleCount(s);

        break;
      }
    }
  }
}

VkShaderModule Renderer::createShader(
  const QString &_filePath,
  VkShaderStageFlagBits _stage
)
{
  QFile file(_filePath);
  if (!file.open(QIODevice::ReadOnly)) {
    qWarning("Failed to read shader %s", qPrintable(_filePath));
    return VK_NULL_HANDLE;
  }
  auto buffer = file.readAll();
  file.close();

//  GLSLCompiler compiler;
//
//  std::vector<uint32_t> spirV;
//  std::string infoLog;
//
//  if(!compiler.compile_to_spirv(_stage, buffer, "main", spirV, infoLog))
//  {
//    qWarning("Failed to compile shader, {}", infoLog.c_str());
//
//    return VK_NULL_HANDLE;
//  }

  VkShaderModuleCreateInfo shaderInfo;
  memset(&shaderInfo, 0, sizeof(shaderInfo));
  shaderInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
  shaderInfo.codeSize = /*spirV*/buffer.size();
  shaderInfo.pCode = reinterpret_cast<const uint32_t *>(buffer.constData());//spirV.data();

  VkShaderModule shaderModule;
  VkResult err = m_deviceFuncs->vkCreateShaderModule(
    m_vkWindow->device(),
    &shaderInfo,
    nullptr,
    &shaderModule
  );
  if (err != VK_SUCCESS)
  {
    qWarning("Failed to create shader module: %d", err);
    return VK_NULL_HANDLE;
  }

  return shaderModule;
}

void Renderer::startNextFrame()
{
  VkCommandBuffer cb = m_vkWindow->currentCommandBuffer();
  const QSize sz = m_vkWindow->swapChainImageSize();

  VkClearColorValue clearColor = {{ 0, 0, 0, 1 }};
  VkClearDepthStencilValue clearDS = { 1, 0 };
  VkClearValue clearValues[3];
  memset(clearValues, 0, sizeof(clearValues));
  clearValues[0].color = clearValues[2].color = clearColor;
  clearValues[1].depthStencil = clearDS;

  VkRenderPassBeginInfo rpBeginInfo;
  memset(&rpBeginInfo, 0, sizeof(rpBeginInfo));
  rpBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
  rpBeginInfo.renderPass = m_vkWindow->defaultRenderPass();
  rpBeginInfo.framebuffer = m_vkWindow->currentFramebuffer();
  rpBeginInfo.renderArea.extent.width = sz.width();
  rpBeginInfo.renderArea.extent.height = sz.height();
  rpBeginInfo.clearValueCount = m_vkWindow->sampleCountFlagBits() > VK_SAMPLE_COUNT_1_BIT ? 3 : 2;
  rpBeginInfo.pClearValues = clearValues;
  VkCommandBuffer cmdBuf = m_vkWindow->currentCommandBuffer();
  m_deviceFuncs->vkCmdBeginRenderPass(cmdBuf, &rpBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

  quint8 *p;
  VkResult err = m_deviceFuncs->vkMapMemory(
    m_device,
    m_bufMem,
    m_uniformBufInfo[m_vkWindow->currentFrame()].offset,
    UNIFORM_DATA_SIZE,
    0,
    reinterpret_cast<void **>(&p)
  );
  if (err != VK_SUCCESS)
    qFatal("Failed to map memory: %d", err);
  QMatrix4x4 m = m_proj;
  m.rotate(m_rotation, 0, 1, 0);
  memcpy(p, m.constData(), 16 * sizeof(float));
  m_deviceFuncs->vkUnmapMemory(m_device, m_bufMem);

  // Not exactly a real animation system, just advance on every frame for now.
  m_rotation += 1.0f;

  m_deviceFuncs->vkCmdBindPipeline(cb, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipeline);
  m_deviceFuncs->vkCmdBindDescriptorSets(cb, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipelineLayout, 0, 1,
                                      &m_descSet[m_vkWindow->currentFrame()], 0, nullptr);
  VkDeviceSize vbOffset = 0;
  m_deviceFuncs->vkCmdBindVertexBuffers(cb, 0, 1, &m_buf, &vbOffset);

  VkViewport viewport;
  viewport.x = viewport.y = 0;
  viewport.width = sz.width();
  viewport.height = sz.height();
  viewport.minDepth = 0;
  viewport.maxDepth = 1;
  m_deviceFuncs->vkCmdSetViewport(cb, 0, 1, &viewport);

  VkRect2D scissor;
  scissor.offset.x = scissor.offset.y = 0;
  scissor.extent.width = viewport.width;
  scissor.extent.height = viewport.height;
  m_deviceFuncs->vkCmdSetScissor(cb, 0, 1, &scissor);

  m_deviceFuncs->vkCmdDraw(cb, 3, 1, 0, 0);

  m_deviceFuncs->vkCmdEndRenderPass(cmdBuf);

  m_vkWindow->frameReady();
  m_vkWindow->requestUpdate(); // render continuously, throttled by the presentation rate

  emit dynamic_cast<VulkanWindow *>(m_vkWindow)->frameQueued(int(m_rotation) % 360);
//  emit dynamic_cast<VulkanWindow *>(m_vkWindow)->nodeEditorModified();
}
