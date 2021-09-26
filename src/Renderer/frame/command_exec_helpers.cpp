/*****************************************************
 * Partial Class: Renderer
 * Members: Frame - Command Execution Helpers (Private)
 *****************************************************/

#include "Renderer.hpp"

using namespace sdfRay4d;

/**
 * @brief generates/initializes and executes command buffers
 * @note per frame
 */
void Renderer::executeCommands()
{
  m_pipelineHelper.command.init(
    m_vkWindow->currentCommandBuffer(),
    m_vkWindow->currentFramebuffer(),
    m_vkWindow->currentFrame(),
    m_windowSize.width(),
    m_windowSize.height()
  );

  // Custom RenderPass with depth attachment only
  m_pipelineHelper.command.executeRenderPass({
    m_depthMaterial // bind material and draw
  });

  /**
   * @note since need to read from the depth buffer in the depth pass shader
   * and command buffers are within the previous render pass, therefore
   * explicit device synchronization is required here.
   */
  m_pipelineHelper.command.executePipelineBarrier(
    m_depthMaterial
  );

  // default Qt Vulkan RenderPass
  m_pipelineHelper.command.executeRenderPass({
    m_actorMaterial, // bind material and draw
    m_sdfrMaterial // bind material and draw
  });
}
