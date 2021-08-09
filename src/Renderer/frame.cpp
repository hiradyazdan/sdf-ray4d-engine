/*****************************************************
 * Partial Class: Renderer
 * Members: Frame Helper Methods (Public/Private)
 *****************************************************/

#include "Renderer.hpp"

using namespace sdfRay4d;

void Renderer::startNextFrame()
{
  Q_ASSERT(!m_isFramePending);
  m_isFramePending = true;

  /**
   * Command buffers handle CPU Workload
   * So, generating command buffers can be offloaded to a CPU worker thread
   */
  auto future = QtConcurrent::run(this, &Renderer::buildFrame);
  m_frameWatcher.setFuture(future);
}

void Renderer::buildFrame()
{
  // To make this function Thread-safe
  QMutexLocker locker(&m_guiMutex);

  createBuffers();
//  ensureInstanceBuffer();
  m_sdfPipelineWorker.waitForFinished();

  cmdRenderPass();
}

void Renderer::updateFrame()
{
  if (m_isFramePending)
  {
    m_isFramePending = false;

    m_vkWindow->frameReady();
    m_vkWindow->requestUpdate();
  }
}
