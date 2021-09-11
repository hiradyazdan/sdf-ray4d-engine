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
   * Qt Vulkan generates command buffers during
   * frame draw so they're hidden away and ready for use
   * per frame, exposing the current command buffer.
   *
   * As command buffers handle CPU Workload,
   * generating command buffers can be
   * offloaded to a CPU worker thread
   */
  const auto &worker = QtConcurrent::run(
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

  m_pipelineHelper.waitForWorkersToFinish();

  executeCommands();
}

void Renderer::updateFrame()
{
  if(!m_isFramePending) return;

  m_isFramePending = false;

  m_vkWindow->frameReady();
  m_vkWindow->requestUpdate();

  /**
   * @note this has to be invoked after
   * frameReady and requestUpdate methods
   * as otherwise it attempts to add to the
   * command buffer that its bound pipeline is
   * already destroyed.
   *
   */
  swapSDFRPipelines();
}
