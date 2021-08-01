/*****************************************************
 * Partial Class: Renderer
 * Members: SwapChain Resources Methods Overrides (Public)
 *****************************************************/

#include "Renderer.hpp"

using namespace sdfRay4d;

void Renderer::initSwapChainResources()
{
  qDebug("initSwapChainResources");

  /**
   * NOTE:
   *
   * The vertex data and the projection matrix assume OpenGL. With
   * Vulkan Y is negated in clip space and the near/far plane is at 0/1 instead
   * of -1/1. These will be corrected for by an extra transformation when
   * calculating the model-view-projection matrix.
   *
   * Y: up, front: CCW
   *
   */
  m_proj = m_vkWindow->clipCorrectionMatrix();
  auto windowSize = m_vkWindow->swapChainImageSize();
  m_proj.perspective(
    45.0f,
    (float) windowSize.width() / (float) windowSize.height(),
    0.01f,
    1000.0f
  );
//  m_proj.translate(0, 0, -4);

  markViewProjDirty();
}

void Renderer::releaseSwapChainResources()
{
  qDebug("releaseSwapChainResources");

  // It is important to finish the pending frame right here since this is the
  // last opportunity to act with all resources intact.
  m_frameWatcher.waitForFinished();
  // Cannot count on the finished() signal being emitted before returning
  // from here.
  if (m_isFramePending) {
    m_isFramePending = false;
    m_vkWindow->frameReady();
  }
}
