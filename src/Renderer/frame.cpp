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
   * So, generating command buffers can be
   * offloaded to a CPU worker thread
   */
  auto worker = QtConcurrent::run(
    this,
    &Renderer::buildFrame
  );
  m_frameWatcher.setFuture(worker);
}

void Renderer::buildFrame()
{
  // makes this function Thread-safe
  QMutexLocker locker(&m_guiMutex);

  createBuffers();
//  ensureInstanceBuffer();
  m_pipelineHelper.waitForWorkersToFinish();

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
