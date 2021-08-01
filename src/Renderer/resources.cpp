/*****************************************************
 * Partial Class: Renderer
 * Members: Resources Methods Overrides (Public)
 *****************************************************/

#include "Renderer.hpp"

using namespace sdfRay4d;

void Renderer::preInitResources()
{
  if(m_isMSAA)
  {
    const auto sampleCounts = m_vkWindow->supportedSampleCounts();
    qDebug() << "Supported sample counts:" << sampleCounts;

    for (int s = 16; s >= 4; s /= 2)
    {
      if (sampleCounts.contains(s))
      {
        qDebug("Requesting sample count %d", s);
        m_vkWindow->setSampleCount(s);

        break;
      }
    }
  }
}

void Renderer::initResources()
{
  qDebug("initResources");

  m_isFramePending = false;

  initVkFunctions();
  initShaders();

  m_pipelinesFuture = QtConcurrent::run(this, &Renderer::createPipelines);
}

void Renderer::releaseResources()
{
  qDebug("releaseResources");

  m_pipelinesFuture.waitForFinished();

  destroyDescriptorSets();
  destroyPipelines();
  destroyBuffers();
  destroyShaderModules();
}
