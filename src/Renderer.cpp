/*****************************************************
 * Partial Class: Renderer (Ctor & General)
 * Members: General Functions (Public/Private)
 *
 * This Class is split into partials to categorize
 * and classify the functionality
 * for the purpose of readability/maintainability
 *
 * The partials can be found in the respective
 * directory named as the class name
 *****************************************************/

#include "Renderer.hpp"

using namespace sdfRay4d;

Renderer::Renderer(
  VulkanWindow *_vkWindow,
  bool _isMSAA
) :
  m_vkWindow(_vkWindow),
  m_isMSAA(_isMSAA)
{
  connect(
    &m_frameWatcher, &QFutureWatcherBase::finished,
    this, &Renderer::updateFrame
  );
}

void Renderer::markViewProjDirty()
{
  m_vpDirty = m_vkWindow->concurrentFrameCount();
}
