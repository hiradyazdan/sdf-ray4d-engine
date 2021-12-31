/*****************************************************
 * Partial Class: MainWindow
 * Members: Constructor Vulkan Window initializers (Private)
 *****************************************************/

#include <QLoggingCategory>

#include "Window/MainWindow.hpp"

using namespace sdfRay4d;

void MainWindow::initVkLayers()
{
  QLoggingCategory::setFilterRules(QStringLiteral("qt.vulkan=true"));

  const QByteArrayList &layers = {
#ifndef Q_OS_ANDROID
    "VK_LAYER_KHRONOS_validation"
#else
    "VK_LAYER_GOOGLE_threading",
    "VK_LAYER_LUNARG_parameter_validation",
    "VK_LAYER_LUNARG_object_tracker",
    "VK_LAYER_LUNARG_core_validation",
    "VK_LAYER_LUNARG_image",
    "VK_LAYER_LUNARG_swapchain",
    "VK_LAYER_GOOGLE_unique_objects"
#endif
  };

  for(const auto &layer : layers)
  {
    if (!m_vkInstance->supportedLayers().contains(layer))
    {
      qFatal("This Validation layer isn't supported: %s", layer.data());
    }
  }

  // Enable validation layer,
  // if supported. Messages go to qDebug by default.
  m_vkInstance->setLayers(layers);
}

void MainWindow::initVkInstance()
{
  m_vkInstance = new QVulkanInstance();

#ifndef NDEBUG
  initVkLayers();
#endif

  if (!m_vkInstance->create())
  {
    qFatal("Failed to create Vulkan instance: %d", m_vkInstance->errorCode());
  }
}

void MainWindow::initVkWindow()
{
  initVkInstance();

  m_vkWindow = new VulkanWindow(true);
  m_vkWindow->setVulkanInstance(m_vkInstance);
}
