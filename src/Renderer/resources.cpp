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
  m_materials.clear();

  initVkFunctions();
  initMaterials();
  initShaders();

  m_pipelineHelper.createCache();
  m_pipelineHelper.createWorkers(m_materials);
}

void Renderer::releaseResources()
{
  qDebug("releaseResources");

  m_pipelineHelper.waitForWorkersToFinish();

  m_pipelineHelper.destroyDescriptors();
  m_pipelineHelper.destroyPipelines();
  m_pipelineHelper.destroyRenderPass();
  m_pipelineHelper.destroyShaderModules();
  m_pipelineHelper.destroyTextures();
  m_pipelineHelper.destroyBuffers();
  m_pipelineHelper.destroyMaterials();
}
