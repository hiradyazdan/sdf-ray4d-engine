/*****************************************************
 * Partial Class: Renderer
 * Members: Init Shaders Helpers (Private)
 *****************************************************/

#include "Renderer.hpp"

using namespace sdfRay4d;

void Renderer::initDepthShaders()
{
  namespace depthShaders = constants::shadersPaths::depth;

  auto &vertexShader = m_depthMaterial->vertexShader;
  auto &fragmentShader = m_depthMaterial->fragmentShader;

  if (!vertexShader.isValid())
  {
    vertexShader.load(QString(depthShaders::vert::mainSPV));
  }

  if (!fragmentShader.isValid())
  {
    fragmentShader.load(QString(depthShaders::frag::mainSPV));
  }
}

void Renderer::initActorShaders()
{
  namespace actorShaders = constants::shadersPaths::actor;

  auto &vertexShader = m_actorMaterial->vertexShader;
  auto &fragmentShader = m_actorMaterial->fragmentShader;

  if (!vertexShader.isValid())
  {
    vertexShader.load(QString(actorShaders::vert::mainSPV));
  }

  if (!fragmentShader.isValid())
  {
    fragmentShader.load(QString(actorShaders::frag::mainSPV));
  }
}

void Renderer::initSDFRShaders()
{
  namespace sdfrShaders = constants::shadersPaths::raymarch;

  auto &vertexShader = m_sdfrMaterial->vertexShader;
  auto &fragmentShader = m_sdfrMaterial->fragmentShader;

  if (!vertexShader.isValid())
  {
    vertexShader.load(QString(sdfrShaders::vert::mainSPV));
  }

  if (!fragmentShader.isValid())
  {
    fragmentShader.load(QString(sdfrShaders::frag::mainSPV));
  }
}
