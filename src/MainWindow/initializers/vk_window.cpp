/*****************************************************
 * Partial Class: MainWindow
 * Members: Constructor Vulkan Window init helpers (Private)
 *****************************************************/

#include "Window/MainWindow.hpp"

using namespace sdfRay4d;

void MainWindow::initVkInstance()
{
  m_vkInstance = new QVulkanInstance();

  /**
   * TODO:
   * Validation layers in Qt seem to be skipped
   * when using Debug mode on CLion
   */
  const QByteArrayList &validationLayers = {
    "VK_LAYER_KHRONOS_validation"
  };

  for(const auto &layer : validationLayers)
  {
    if (!m_vkInstance->supportedLayers().contains(layer))
    {
      qFatal("This Validation layer isn't supported: " + layer);
    }
  }

  // Enable validation layer,
  // if supported. Messages go to qDebug by default.
  m_vkInstance->setLayers(validationLayers);

  if (!m_vkInstance->create())
  {
    qFatal("Failed to create Vulkan instance: %d", m_vkInstance->errorCode());
  }
}

void MainWindow::initVkWindow()
{
  initVkInstance();

  m_vkWindow = new VulkanWindow();
  m_vkWindow->setVulkanInstance(m_vkInstance);
}
