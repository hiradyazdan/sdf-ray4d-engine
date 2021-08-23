/*****************************************************
 * Partial Class: Renderer
 * Members: Init Shaders Helpers (Private)
 *****************************************************/

#include "Renderer.hpp"

using namespace sdfRay4d;

void Renderer::initDepthShaders()
{
  const auto &depthShaders = m_appConstants.shadersPaths.depth;

  if (!m_depthMaterial->vertexShader.isValid())
  {
    m_depthMaterial->vertexShader.load(
      depthShaders.vert.mainSPV, {}
    );
  }

  if (!m_depthMaterial->fragmentShader.isValid())
  {
    m_depthMaterial->fragmentShader.load(
      depthShaders.frag.mainSPV, {}
    );
  }
}

void Renderer::initActorShaders()
{
  const auto &actorShaders = m_appConstants.shadersPaths.actor;

  if (!m_actorMaterial->vertexShader.isValid())
  {
    m_actorMaterial->vertexShader.load(
      actorShaders.vert.mainSPV, {}
    );
  }

  if (!m_actorMaterial->fragmentShader.isValid())
  {
    m_actorMaterial->fragmentShader.load(
      actorShaders.frag.mainSPV, {}
    );
  }
}

void Renderer::initSDFRShaders()
{
  const auto &sdfrShaders = m_appConstants.shadersPaths.raymarch;

  if (!m_sdfrMaterial->vertexShader.isValid())
  {
    m_sdfrMaterial->vertexShader.load(
      sdfrShaders.vert.mainSPV, {}
    );
  }

  if (!m_sdfrMaterial->fragmentShader.isValid())
  {
    m_sdfrMaterial->fragmentShader.load(
      sdfrShaders.frag.mainSPV, {}
    );
  }
}
