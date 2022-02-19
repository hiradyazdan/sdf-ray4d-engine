/*****************************************************
 * Partial Class: Window (General)
 * Members: General Functions (Public)
 *
 * This Class is split into partials to categorize
 * and classify the functionality
 * for the purpose of readability/maintainability
 *
 * The partials can be found in the respective
 * directory named as the class name
 *
 * Partials:
 *****************************************************/

#include "Window/VulkanWindow.hpp"

using namespace sdfRay4d;

/**
 *
 * @param[in] _isDebug
 */
VulkanWindow::VulkanWindow(
  bool _isDebug
) :
  m_isDebug(_isDebug)
{}

QVulkanWindowRenderer *VulkanWindow::createRenderer()
{
  /**
   * @note
   *
   * QVulkanWindowPrivate init function calls this function earlier than
   * creating the device with vkCreateDevice. On initial load of the window,
   * any device related functionality will only be available through resource
   * initializers i.e., initResources() and not even preInitResources()
   *
   * This forces the material struct instance to be made a pointer, for
   * two reasons:
   *
   * 1. it cannot be initialized before resource initializers
   * 2. it has a non-default ctor, so its member needs to be initialized
   *    in a ctor's initializer list, instead of ctor body or member functions
   *    that may be called when device/device functions are already available.
   *
   * Also in a larger scale, we need access to material between multiple threads,
   * and therefore a reference counted pointer at minimum is needed to keep its track
   */

  m_renderer = new Renderer(
    this,
    false //true @todo temporarily disable msaa for sampling depth texture
  );

  return m_renderer;
}

/**
 *
 * @param[in] _isNew
 * @return MaterialPtr instance
 */
VulkanWindow::MaterialPtr &VulkanWindow::getSDFRMaterial(bool _isNew)
{
  return m_renderer->getSDFRMaterial(_isNew);
}

void VulkanWindow::createSDFRPipeline()
{
  m_renderer->createSDFRPipeline();
}
