/*****************************************************
 * Partial Class: Renderer (Ctor & General)
 * Members: General Functions (Public/Private)
 *
 * This Class is split into partials to categorize
 * and classify the functionality
 * for the purpose of readability/maintainability
 *
 * The partials can be found in the respective
 * directory named as the class name
 *
 * Partials:
 * - resources (resources.cpp)
 *      - init_helpers.cpp
 *      - init_materials_helpers.cpp
 *      - init_shaders_helpers.cpp
 *      - sdf_graph_pipeline_helpers.cpp
 * - frame (frame.cpp)
 *      - buffers.cpp
 *      - command_exec_helpers.cpp
 * - swapchain_resources.cpp
 * - user_input_helpers.cpp
 *****************************************************/

#include "Renderer.hpp"

using namespace sdfRay4d;

/**
 *
 * @param[in] _vkWindow
 * @param[in] _isMSAA
 */
Renderer::Renderer(
  VulkanWindow *_vkWindow,
  bool _isMSAA
) :
  m_vkWindow(_vkWindow)
, m_isMSAA(_isMSAA)
{
  m_actorMesh.load(
    QString(constants::modelsPath) +
      QString(constants::modelsPaths::actor)
  );

  connect(
    &m_frameWatcher, &QFutureWatcherBase::finished,
    this, &Renderer::updateFrame
  );
}

void Renderer::markViewProjDirty()
{
  m_concurrentFrameCount = m_vkWindow->concurrentFrameCount();
}
