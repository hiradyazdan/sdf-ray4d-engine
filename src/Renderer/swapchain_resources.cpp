/*****************************************************
 * Partial Class: Renderer
 * Members: SwapChain Resources Methods Overrides (Public)
 *****************************************************/

#include "Renderer.hpp"

using namespace sdfRay4d;

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
