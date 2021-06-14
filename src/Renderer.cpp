#include <QVulkanFunctions>

//#include "GLSLCompiler.hpp"
#include "Renderer.hpp"

using namespace ray4d;

// Note that the vertex data and the projection matrix assume OpenGL. With
// Vulkan Y is negated in clip space and the near/far plane is at 0/1 instead
// of -1/1. These will be corrected for by an extra transformation when
// calculating the modelview-projection matrix.
static float vertexData[] = { // Y up, front = CCW
  0.0f,   0.5f,   1.0f, 0.0f, 0.0f,
  -0.5f,  -0.5f,   0.0f, 1.0f, 0.0f,
  0.5f,  -0.5f,   0.0f, 0.0f, 1.0f
};

static const int UNIFORM_DATA_SIZE = 16 * sizeof(float);

static inline VkDeviceSize aligned(VkDeviceSize v, VkDeviceSize byteAlign)
{
  return (v + byteAlign - 1) & ~(byteAlign - 1);
}

Renderer::Renderer(QVulkanWindow *_vkWindow, bool _msaa) : m_vkWindow(_vkWindow)
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

VkShaderModule Renderer::createShader(const QString &_filePath, VkShaderStageFlagBits _stage)
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
  VkResult err = m_devFuncs->vkCreateShaderModule(
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

void Renderer::initResources()
{
  qDebug("initResources");

  VkDevice dev = m_vkWindow->device();
  QVulkanInstance *inst = m_vkWindow->vulkanInstance();
  m_devFuncs = inst->deviceFunctions(dev);

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

  const int concurrentFrameCount = m_vkWindow->concurrentFrameCount();
  const VkPhysicalDeviceLimits *pdevLimits = &m_vkWindow->physicalDeviceProperties()->limits;
  const VkDeviceSize uniAlign = pdevLimits->minUniformBufferOffsetAlignment;
  qDebug("uniform buffer offset alignment is %u", (uint) uniAlign);
  VkBufferCreateInfo bufInfo;
  memset(&bufInfo, 0, sizeof(bufInfo));
  bufInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
  // Our internal layout is vertex, uniform, uniform, ... with each uniform buffer start offset aligned to uniAlign.
  const VkDeviceSize vertexAllocSize = aligned(sizeof(vertexData), uniAlign);
  const VkDeviceSize uniformAllocSize = aligned(UNIFORM_DATA_SIZE, uniAlign);
  bufInfo.size = vertexAllocSize + concurrentFrameCount * uniformAllocSize;
  bufInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;

  VkResult err = m_devFuncs->vkCreateBuffer(dev, &bufInfo, nullptr, &m_buf);
  if (err != VK_SUCCESS)
    qFatal("Failed to create buffer: %d", err);

  VkMemoryRequirements memReq;
  m_devFuncs->vkGetBufferMemoryRequirements(dev, m_buf, &memReq);

  VkMemoryAllocateInfo memAllocInfo = {
    VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
    nullptr,
    memReq.size,
    m_vkWindow->hostVisibleMemoryIndex()
  };

  err = m_devFuncs->vkAllocateMemory(dev, &memAllocInfo, nullptr, &m_bufMem);
  if (err != VK_SUCCESS)
    qFatal("Failed to allocate memory: %d", err);

  err = m_devFuncs->vkBindBufferMemory(dev, m_buf, m_bufMem, 0);
  if (err != VK_SUCCESS)
    qFatal("Failed to bind buffer memory: %d", err);

  quint8 *p;
  err = m_devFuncs->vkMapMemory(dev, m_bufMem, 0, memReq.size, 0, reinterpret_cast<void **>(&p));
  if (err != VK_SUCCESS)
    qFatal("Failed to map memory: %d", err);
  memcpy(p, vertexData, sizeof(vertexData));
  QMatrix4x4 ident;
  memset(m_uniformBufInfo, 0, sizeof(m_uniformBufInfo));
  for (int i = 0; i < concurrentFrameCount; ++i) {
    const VkDeviceSize offset = vertexAllocSize + i * uniformAllocSize;
    memcpy(p + offset, ident.constData(), 16 * sizeof(float));
    m_uniformBufInfo[i].buffer = m_buf;
    m_uniformBufInfo[i].offset = offset;
    m_uniformBufInfo[i].range = uniformAllocSize;
  }
  m_devFuncs->vkUnmapMemory(dev, m_bufMem);

  VkVertexInputBindingDescription vertexBindingDesc = {
    0, // binding
    5 * sizeof(float),
    VK_VERTEX_INPUT_RATE_VERTEX
  };
  VkVertexInputAttributeDescription vertexAttrDesc[] = {
    { // position
      0, // location
      0, // binding
      VK_FORMAT_R32G32_SFLOAT,
      0
    },
    { // color
      1,
      0,
      VK_FORMAT_R32G32B32_SFLOAT,
      2 * sizeof(float)
    }
  };

  VkPipelineVertexInputStateCreateInfo vertexInputInfo;
  vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
  vertexInputInfo.pNext = nullptr;
  vertexInputInfo.flags = 0;
  vertexInputInfo.vertexBindingDescriptionCount = 1;
  vertexInputInfo.pVertexBindingDescriptions = &vertexBindingDesc;
  vertexInputInfo.vertexAttributeDescriptionCount = 2;
  vertexInputInfo.pVertexAttributeDescriptions = vertexAttrDesc;

  // Set up descriptor set and its layout.
  VkDescriptorPoolSize descPoolSizes = { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, uint32_t(concurrentFrameCount) };
  VkDescriptorPoolCreateInfo descPoolInfo;
  memset(&descPoolInfo, 0, sizeof(descPoolInfo));
  descPoolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
  descPoolInfo.maxSets = concurrentFrameCount;
  descPoolInfo.poolSizeCount = 1;
  descPoolInfo.pPoolSizes = &descPoolSizes;
  err = m_devFuncs->vkCreateDescriptorPool(dev, &descPoolInfo, nullptr, &m_descPool);
  if (err != VK_SUCCESS)
    qFatal("Failed to create descriptor pool: %d", err);

  VkDescriptorSetLayoutBinding layoutBinding = {
    0, // binding
    VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
    1,
    VK_SHADER_STAGE_VERTEX_BIT,
    nullptr
  };
  VkDescriptorSetLayoutCreateInfo descLayoutInfo = {
    VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
    nullptr,
    0,
    1,
    &layoutBinding
  };
  err = m_devFuncs->vkCreateDescriptorSetLayout(dev, &descLayoutInfo, nullptr, &m_descSetLayout);
  if (err != VK_SUCCESS)
    qFatal("Failed to create descriptor set layout: %d", err);

  for (int i = 0; i < concurrentFrameCount; ++i) {
    VkDescriptorSetAllocateInfo descSetAllocInfo = {
      VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
      nullptr,
      m_descPool,
      1,
      &m_descSetLayout
    };
    err = m_devFuncs->vkAllocateDescriptorSets(dev, &descSetAllocInfo, &m_descSet[i]);
    if (err != VK_SUCCESS)
      qFatal("Failed to allocate descriptor set: %d", err);

    VkWriteDescriptorSet descWrite;
    memset(&descWrite, 0, sizeof(descWrite));
    descWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descWrite.dstSet = m_descSet[i];
    descWrite.descriptorCount = 1;
    descWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    descWrite.pBufferInfo = &m_uniformBufInfo[i];
    m_devFuncs->vkUpdateDescriptorSets(dev, 1, &descWrite, 0, nullptr);
  }

  // Pipeline cache
  VkPipelineCacheCreateInfo pipelineCacheInfo;
  memset(&pipelineCacheInfo, 0, sizeof(pipelineCacheInfo));
  pipelineCacheInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
  err = m_devFuncs->vkCreatePipelineCache(dev, &pipelineCacheInfo, nullptr, &m_pipelineCache);
  if (err != VK_SUCCESS)
    qFatal("Failed to create pipeline cache: %d", err);

  // Pipeline layout
  VkPipelineLayoutCreateInfo pipelineLayoutInfo;
  memset(&pipelineLayoutInfo, 0, sizeof(pipelineLayoutInfo));
  pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
  pipelineLayoutInfo.setLayoutCount = 1;
  pipelineLayoutInfo.pSetLayouts = &m_descSetLayout;
  err = m_devFuncs->vkCreatePipelineLayout(dev, &pipelineLayoutInfo, nullptr, &m_pipelineLayout);
  if (err != VK_SUCCESS)
    qFatal("Failed to create pipeline layout: %d", err);

  // Shaders
  VkShaderModule vertShaderModule = createShader(
    QStringLiteral("assets/shaders/color_vert.spv"),
    VK_SHADER_STAGE_VERTEX_BIT
  );
  VkShaderModule fragShaderModule = createShader(
    QStringLiteral("assets/shaders/color_frag.spv"),
    VK_SHADER_STAGE_FRAGMENT_BIT
  );

  // Graphics pipeline
  VkGraphicsPipelineCreateInfo pipelineInfo;
  memset(&pipelineInfo, 0, sizeof(pipelineInfo));
  pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;

  VkPipelineShaderStageCreateInfo shaderStages[2] = {
    {
      VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
      nullptr,
      0,
      VK_SHADER_STAGE_VERTEX_BIT,
      vertShaderModule,
      "main",
      nullptr
    },
    {
      VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
      nullptr,
      0,
      VK_SHADER_STAGE_FRAGMENT_BIT,
      fragShaderModule,
      "main",
      nullptr
    }
  };
  pipelineInfo.stageCount = 2;
  pipelineInfo.pStages = shaderStages;

  pipelineInfo.pVertexInputState = &vertexInputInfo;

  VkPipelineInputAssemblyStateCreateInfo ia;
  memset(&ia, 0, sizeof(ia));
  ia.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
  ia.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
  pipelineInfo.pInputAssemblyState = &ia;

  // The viewport and scissor will be set dynamically via vkCmdSetViewport/Scissor.
  // This way the pipeline does not need to be touched when resizing the window.
  VkPipelineViewportStateCreateInfo vp;
  memset(&vp, 0, sizeof(vp));
  vp.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
  vp.viewportCount = 1;
  vp.scissorCount = 1;
  pipelineInfo.pViewportState = &vp;

  VkPipelineRasterizationStateCreateInfo rs;
  memset(&rs, 0, sizeof(rs));
  rs.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
  rs.polygonMode = VK_POLYGON_MODE_FILL;
  rs.cullMode = VK_CULL_MODE_NONE; // we want the back face as well
  rs.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
  rs.lineWidth = 1.0f;
  pipelineInfo.pRasterizationState = &rs;

  VkPipelineMultisampleStateCreateInfo ms;
  memset(&ms, 0, sizeof(ms));
  ms.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
  // Enable multisampling.
  ms.rasterizationSamples = m_vkWindow->sampleCountFlagBits();
  pipelineInfo.pMultisampleState = &ms;

  VkPipelineDepthStencilStateCreateInfo ds;
  memset(&ds, 0, sizeof(ds));
  ds.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
  ds.depthTestEnable = VK_TRUE;
  ds.depthWriteEnable = VK_TRUE;
  ds.depthCompareOp = VK_COMPARE_OP_LESS_OR_EQUAL;
  pipelineInfo.pDepthStencilState = &ds;

  VkPipelineColorBlendStateCreateInfo cb;
  memset(&cb, 0, sizeof(cb));
  cb.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
  // no blend, write out all of rgba
  VkPipelineColorBlendAttachmentState att;
  memset(&att, 0, sizeof(att));
  att.colorWriteMask = 0xF;
  cb.attachmentCount = 1;
  cb.pAttachments = &att;
  pipelineInfo.pColorBlendState = &cb;

  VkDynamicState dynEnable[] = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };
  VkPipelineDynamicStateCreateInfo dyn;
  memset(&dyn, 0, sizeof(dyn));
  dyn.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
  dyn.dynamicStateCount = sizeof(dynEnable) / sizeof(VkDynamicState);
  dyn.pDynamicStates = dynEnable;
  pipelineInfo.pDynamicState = &dyn;

  pipelineInfo.layout = m_pipelineLayout;
  pipelineInfo.renderPass = m_vkWindow->defaultRenderPass();

  err = m_devFuncs->vkCreateGraphicsPipelines(dev, m_pipelineCache, 1, &pipelineInfo, nullptr, &m_pipeline);
  if (err != VK_SUCCESS)
    qFatal("Failed to create graphics pipeline: %d", err);

  if (vertShaderModule) m_devFuncs->vkDestroyShaderModule(dev, vertShaderModule, nullptr);
  if (fragShaderModule) m_devFuncs->vkDestroyShaderModule(dev, fragShaderModule, nullptr);

  m_devFuncs = inst->deviceFunctions(m_vkWindow->device());

  QString info;
  info += QString::asprintf("Number of physical devices: %d\n", int(m_vkWindow->availablePhysicalDevices().count()));

  QVulkanFunctions *f = inst->functions();
  VkPhysicalDeviceProperties props;
  f->vkGetPhysicalDeviceProperties(m_vkWindow->physicalDevice(), &props);
  info += QString::asprintf("Active physical device name: '%s' version %d.%d.%d\nAPI version %d.%d.%d\n",
                            props.deviceName,
                            VK_VERSION_MAJOR(props.driverVersion), VK_VERSION_MINOR(props.driverVersion),
                            VK_VERSION_PATCH(props.driverVersion),
                            VK_VERSION_MAJOR(props.apiVersion), VK_VERSION_MINOR(props.apiVersion),
                            VK_VERSION_PATCH(props.apiVersion));

  info += QStringLiteral("Supported instance layers:\n");
  for (const QVulkanLayer &layer : inst->supportedLayers())
    info += QString::asprintf("    %s v%u\n", layer.name.constData(), layer.version);
  info += QStringLiteral("Enabled instance layers:\n");
  for (const QByteArray &layer : inst->layers())
    info += QString::asprintf("    %s\n", layer.constData());

  info += QStringLiteral("Supported instance extensions:\n");
  for (const QVulkanExtension &ext : inst->supportedExtensions())
    info += QString::asprintf("    %s v%u\n", ext.name.constData(), ext.version);
  info += QStringLiteral("Enabled instance extensions:\n");
  for (const QByteArray &ext : inst->extensions())
    info += QString::asprintf("    %s\n", ext.constData());

  info += QString::asprintf("Color format: %u\nDepth-stencil format: %u\n",
                            m_vkWindow->colorFormat(), m_vkWindow->depthStencilFormat());

  info += QStringLiteral("Supported sample counts:");
  const QVector<int> sampleCounts = m_vkWindow->supportedSampleCounts();
  for (int count : sampleCounts)
    info += QLatin1Char(' ') + QString::number(count);
  info += QLatin1Char('\n');

  emit dynamic_cast<VulkanWindow *>(m_vkWindow)->vulkanInfoReceived(info);
}

void Renderer::initSwapChainResources()
{
  qDebug("initSwapChainResources");

  // Projection matrix
  m_proj = m_vkWindow->clipCorrectionMatrix(); // adjust for Vulkan-OpenGL clip space differences
  const QSize sz = m_vkWindow->swapChainImageSize();
  m_proj.perspective(45.0f, sz.width() / (float) sz.height(), 0.01f, 100.0f);
  m_proj.translate(0, 0, -4);
}

void Renderer::releaseSwapChainResources()
{
  qDebug("releaseSwapChainResources");
}

void Renderer::releaseResources()
{
  qDebug("releaseResources");

  VkDevice dev = m_vkWindow->device();

  if (m_pipeline) {
    m_devFuncs->vkDestroyPipeline(dev, m_pipeline, nullptr);
    m_pipeline = VK_NULL_HANDLE;
  }

  if (m_pipelineLayout) {
    m_devFuncs->vkDestroyPipelineLayout(dev, m_pipelineLayout, nullptr);
    m_pipelineLayout = VK_NULL_HANDLE;
  }

  if (m_pipelineCache) {
    m_devFuncs->vkDestroyPipelineCache(dev, m_pipelineCache, nullptr);
    m_pipelineCache = VK_NULL_HANDLE;
  }

  if (m_descSetLayout) {
    m_devFuncs->vkDestroyDescriptorSetLayout(dev, m_descSetLayout, nullptr);
    m_descSetLayout = VK_NULL_HANDLE;
  }

  if (m_descPool) {
    m_devFuncs->vkDestroyDescriptorPool(dev, m_descPool, nullptr);
    m_descPool = VK_NULL_HANDLE;
  }

  if (m_buf) {
    m_devFuncs->vkDestroyBuffer(dev, m_buf, nullptr);
    m_buf = VK_NULL_HANDLE;
  }

  if (m_bufMem) {
    m_devFuncs->vkFreeMemory(dev, m_bufMem, nullptr);
    m_bufMem = VK_NULL_HANDLE;
  }
}

void Renderer::startNextFrame()
{
  VkDevice dev = m_vkWindow->device();
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
  m_devFuncs->vkCmdBeginRenderPass(cmdBuf, &rpBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

  quint8 *p;
  VkResult err = m_devFuncs->vkMapMemory(
    dev,
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
  m_devFuncs->vkUnmapMemory(dev, m_bufMem);

  // Not exactly a real animation system, just advance on every frame for now.
  m_rotation += 1.0f;

  m_devFuncs->vkCmdBindPipeline(cb, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipeline);
  m_devFuncs->vkCmdBindDescriptorSets(cb, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipelineLayout, 0, 1,
                                      &m_descSet[m_vkWindow->currentFrame()], 0, nullptr);
  VkDeviceSize vbOffset = 0;
  m_devFuncs->vkCmdBindVertexBuffers(cb, 0, 1, &m_buf, &vbOffset);

  VkViewport viewport;
  viewport.x = viewport.y = 0;
  viewport.width = sz.width();
  viewport.height = sz.height();
  viewport.minDepth = 0;
  viewport.maxDepth = 1;
  m_devFuncs->vkCmdSetViewport(cb, 0, 1, &viewport);

  VkRect2D scissor;
  scissor.offset.x = scissor.offset.y = 0;
  scissor.extent.width = viewport.width;
  scissor.extent.height = viewport.height;
  m_devFuncs->vkCmdSetScissor(cb, 0, 1, &scissor);

  m_devFuncs->vkCmdDraw(cb, 3, 1, 0, 0);

  m_devFuncs->vkCmdEndRenderPass(cmdBuf);

  m_vkWindow->frameReady();
  m_vkWindow->requestUpdate(); // render continuously, throttled by the presentation rate

  emit dynamic_cast<VulkanWindow *>(m_vkWindow)->frameQueued(int(m_rotation) % 360);
}
