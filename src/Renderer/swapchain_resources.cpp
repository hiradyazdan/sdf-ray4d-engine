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
   * @note
   *
   * The vertex data and the projection matrix assume OpenGL. With
   * Vulkan Y is negated in clip space and the near/far plane is at 0/1 instead
   * of -1/1. These will be corrected for by an extra transformation when
   * calculating the model-view-projection matrix.
   *
   * Y: up, front: CCW
   *
   */
  m_windowSize = m_vkWindow->swapChainImageSize();
  auto aspectRatio = (float) m_windowSize.width() / (float) m_windowSize.height();

  m_proj = m_vkWindow->clipCorrectionMatrix();
  m_proj.perspective(
    m_verticalAngle,
    aspectRatio,
    m_nearPlane, m_farPlane
  );
//  m_proj.translate(0, 0, -4);

  markViewProjDirty();

  createDepthView();

  m_pipelineHelper.initSwapChainHelpers(
    m_depthMaterial->texture.getImageView() // framebuffer depth attachment for custom renderPass
  );
}

void Renderer::releaseSwapChainResources()
{
  qDebug("releaseSwapChainResources");

  m_frameWatcher.waitForFinished();

  if (m_isFramePending)
  {
    m_isFramePending = false;
    m_vkWindow->frameReady();
  }
}
