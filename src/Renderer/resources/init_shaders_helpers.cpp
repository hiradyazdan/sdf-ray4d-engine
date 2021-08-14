/*****************************************************
 * Partial Class: Renderer
 * Members: Init Shaders Helpers (Private)
 *****************************************************/

#include "Renderer.hpp"

using namespace sdfRay4d;

void Renderer::initSDFRShaders()
{
  const auto &raymarchShaders = m_appConstants.shadersPaths.raymarch;

  if (!m_sdfrMaterial->vertexShader.isValid())
  {
    m_sdfrMaterial->vertexShader.load(
      raymarchShaders.vert.main, {}
      //      "static/screenQuad.vert", {}
    );
  }

  if (!m_sdfrMaterial->fragmentShader.isValid())
  {
    m_sdfrMaterial->fragmentShader.load(
      raymarchShaders.frag.main,
      {
        raymarchShaders.frag.partials.distanceFuncs,
        raymarchShaders.frag.partials.operations
      }
      //      "static/blank.frag", {}
    );
  }
}

void Renderer::initActorShaders()
{
  //  PushConstantRange pushConstant;
  //  pushConstant.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
  //  pushConstant.offset = 0;
  //  pushConstant.size = 64;
  //
  //  m_actorMaterial->pushConstantRangeCount = 1;
  //  m_actorMaterial->pushConstantRange = pushConstant;

  const auto &actorShaders = m_appConstants.shadersPaths.actor;

  if (!m_actorMaterial->vertexShader.isValid())
  {
    m_actorMaterial->vertexShader.load(
      actorShaders.vert.main, {}
      //      "static/screenQuad.vert", {}
    );
  }

  if (!m_actorMaterial->fragmentShader.isValid())
  {
    m_actorMaterial->fragmentShader.load(
      actorShaders.frag.main, {}
    );
  }
}
