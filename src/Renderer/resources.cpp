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
  initMaterials();
  initShaders();

  m_pipelineHelper.createCache();
  m_pipelineHelper.createWorkers({
    m_sdfrMaterial,
    m_actorMaterial
  });
}

void Renderer::releaseResources()
{
  qDebug("releaseResources");

  /**
   * @note
   *
   * This may not be necessary since SDF Pipeline is already created
   * before opening the SDF Graph (race condition was avoided by UI design)
   */
  m_pipelineHelper.waitForWorkersToFinish();

  m_pipelineHelper.destroyDescriptors();
  m_pipelineHelper.destroyPipelines();

  destroyBuffers();
  destroyShaderModules();
}
