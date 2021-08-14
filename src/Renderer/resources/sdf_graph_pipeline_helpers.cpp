/*****************************************************
 * Partial Class: Renderer
 * Members: Pipeline Helpers (Private)
 *****************************************************/

#include "Renderer.hpp"

using namespace sdfRay4d;

void Renderer::createSDFPipeline()
{
  m_pipelineHelper.createWorker(m_sdfrMaterial);
}

void Renderer::destroySDFPipeline()
{
  if (m_isFramePending)
  {
    m_isFramePending = false;
    m_vkWindow->frameReady();
  }

  m_pipelineHelper.destroyPipeline(m_sdfrMaterial);
  m_pipelineHelper.destroyPipelineLayout(m_sdfrMaterial);

  destroyShaderModule(m_sdfrMaterial->fragmentShader);
}
