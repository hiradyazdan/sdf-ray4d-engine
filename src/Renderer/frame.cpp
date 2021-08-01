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

  auto future = QtConcurrent::run(this, &Renderer::buildFrame);
  m_frameWatcher.setFuture(future);
}

void Renderer::buildFrame()
{
//  QMutexLocker locker(&m_guiMutex);

  createBuffers();
//  ensureInstanceBuffer();
  m_pipelinesFuture.waitForFinished();

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
