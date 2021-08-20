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
 *      - destroy_helpers.cpp
 *      - init_helpers.cpp
 *      - init_materials_helpers.cpp
 *      - init_shaders_helpers.cpp
 *      - sdf_graph_pipeline_helpers.cpp
 * - frame (frame.cpp)
 *      - buffers.cpp
 *      - draw_calls.cpp
 *      - render_pass.cpp
 *      - user_input_helpers.cpp
 * - swapchain_resources.cpp
 *****************************************************/

#include "Renderer.hpp"

using namespace sdfRay4d;

Renderer::Renderer(
  VulkanWindow *_vkWindow,
  bool _isMSAA
) :
  m_vkWindow(_vkWindow)
  , m_isMSAA(_isMSAA)
{
  m_actorMesh.load(m_appConstants.assetsPath + "models/" + m_appConstants.modelsPaths.actor);

  connect(
    &m_frameWatcher, &QFutureWatcherBase::finished,
    this, &Renderer::updateFrame
  );
}

void Renderer::markViewProjDirty()
{
  m_vpDirty = m_vkWindow->concurrentFrameCount();
}

/**
 * @brief creates depth image view (texture)
 * to be used for depth/z-buffer calculation and use between passes
 */
void Renderer::createDepthView()
{
  if(m_depthView) return;

  auto &depthTexture = m_depthMaterial->texture;

  depthTexture.createImage(
    m_windowSize.width(),
    m_windowSize.height(),
    VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT |
    VK_IMAGE_USAGE_SAMPLED_BIT
  );
  depthTexture.createImageMemory(
    m_vkWindow->deviceLocalMemoryIndex(),
    m_imageBufferMemory
  );
  depthTexture.createImageView(
    VK_IMAGE_ASPECT_DEPTH_BIT,
    &m_depthView
  );
}
