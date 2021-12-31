/*****************************************************
 * Partial Class: Renderer
 * Members: Init Shaders Helpers (Private)
 *****************************************************/

#include "Renderer.hpp"

using namespace sdfRay4d;

void Renderer::initDepthShaders()
{
  namespace depthShaders = constants::shadersPaths::depth;

  if (!m_depthMaterial->vertexShader.isValid())
  {
    m_depthMaterial->vertexShader.load(
      QString(depthShaders::vert::mainSPV)
    );
  }

  if (!m_depthMaterial->fragmentShader.isValid())
  {
    m_depthMaterial->fragmentShader.load(
      QString(depthShaders::frag::mainSPV)
    );
  }
}

void Renderer::initActorShaders()
{
  namespace actorShaders = constants::shadersPaths::actor;

  if (!m_actorMaterial->vertexShader.isValid())
  {
    m_actorMaterial->vertexShader.load(
      QString(actorShaders::vert::mainSPV)
    );
  }

  if (!m_actorMaterial->fragmentShader.isValid())
  {
    m_actorMaterial->fragmentShader.load(
      QString(actorShaders::frag::mainSPV)
    );
  }
}

void Renderer::initSDFRShaders()
{
  namespace sdfrShaders = constants::shadersPaths::raymarch;

  if (!m_sdfrMaterial->vertexShader.isValid())
  {
    m_sdfrMaterial->vertexShader.load(
      QString(sdfrShaders::vert::mainSPV)
    );
  }

  if (!m_sdfrMaterial->fragmentShader.isValid())
  {
    m_sdfrMaterial->fragmentShader.load(
      QString(sdfrShaders::frag::mainSPV)
    );
  }
}
