/*****************************************************
 * Partial Class: Renderer
 * Members: Resources Methods Overrides (Public)
 *****************************************************/

#include "Renderer.hpp"

using namespace sdfRay4d;

void Renderer::initResources()
{
  qDebug("initResources");

  m_device = m_vkWindow->device();
  auto *vkInstance = m_vkWindow->vulkanInstance();
  m_deviceFuncs = vkInstance->deviceFunctions(m_device);

  const int concurrentFrameCount = m_vkWindow->concurrentFrameCount();

  createUploadBuffer(concurrentFrameCount);
  setupDescriptorSets(concurrentFrameCount);
  createPipelineCache();
  createPipelineLayout();
  createGraphicsPipeline();

  QString info;
  info += QString::asprintf("Number of physical devices: %d\n", int(m_vkWindow->availablePhysicalDevices().count()));

  QVulkanFunctions *f = vkInstance->functions();
  VkPhysicalDeviceProperties props;
  f->vkGetPhysicalDeviceProperties(m_vkWindow->physicalDevice(), &props);
  info += QString::asprintf(
    "Active physical device name: '%s' version %d.%d.%d\nAPI version %d.%d.%d\n",
    props.deviceName,
    VK_VERSION_MAJOR(props.driverVersion), VK_VERSION_MINOR(props.driverVersion),
    VK_VERSION_PATCH(props.driverVersion),
    VK_VERSION_MAJOR(props.apiVersion), VK_VERSION_MINOR(props.apiVersion),
    VK_VERSION_PATCH(props.apiVersion)
  );

  info += QStringLiteral("Supported instance layers:\n");
  for (const QVulkanLayer &layer : vkInstance->supportedLayers())
    info += QString::asprintf("    %s v%u\n", layer.name.constData(), layer.version);
  info += QStringLiteral("Enabled instance layers:\n");
  for (const QByteArray &layer : vkInstance->layers())
    info += QString::asprintf("    %s\n", layer.constData());

  info += QStringLiteral("Supported instance extensions:\n");
  for (const QVulkanExtension &ext : vkInstance->supportedExtensions())
    info += QString::asprintf("    %s v%u\n", ext.name.constData(), ext.version);
  info += QStringLiteral("Enabled instance extensions:\n");
  for (const QByteArray &ext : vkInstance->extensions())
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

void Renderer::releaseResources()
{
  qDebug("releaseResources");

  destroyPipeline();
  destroyDescriptorSets();
  destroyBuffers();
}
