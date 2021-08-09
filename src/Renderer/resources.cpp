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

  m_objMaterial = std::make_shared<Material>(
    m_device,
    m_deviceFuncs
  );

  m_sdfMaterial = std::make_shared<Material>(
    m_device,
    m_deviceFuncs
  );

  m_materials = {
    m_objMaterial,
    m_sdfMaterial
  };

  initSDFShaders();

  createPipelineCache();
  createPipelineWorker(m_sdfPipelineWorker, m_sdfMaterial);
}

void Renderer::releaseResources()
{
  qDebug("releaseResources");

  /**
   * NOTE:
   *
   * This may not be necessary since SDF Pipeline is already created
   * before opening the SDF Graph (race condition was avoided by UI design)
   */
  m_sdfPipelineWorker.waitForFinished();

  destroyDescriptorSets();
  destroyPipelines();
  destroyBuffers();
  destroyShaderModules();
}
