/*****************************************************
 * Partial Class: Renderer
 * Members: SDF Graph Material Pipeline Helpers (Private)
 *****************************************************/

#include "Renderer.hpp"

using namespace sdfRay4d;

/**
 * @brief creates and passes a fresh new material
 * to be consumed by the SDF Graph
 * @return MaterialPtr
 */
Renderer::MaterialPtr &Renderer::getSDFRMaterial(bool _isNew)
{
  if(!_isNew) return m_newSDFRMaterial;

  m_newSDFRMaterial = std::make_shared<Material>(
    m_device,
    m_deviceFuncs
  );

  initSDFRMaterial(m_newSDFRMaterial);

  return m_newSDFRMaterial;
}

void Renderer::createSDFRPipeline()
{
  const auto &newMaterial = getSDFRMaterial();

  m_isNewWorker = true;
  m_pipelineHelper.createWorker(
    newMaterial,
    m_isNewWorker
  );
}

/**
 * @note this function is constantly invoked
 * in the render loop (per frame)
 */
void Renderer::swapSDFRPipelines()
{
  if(!m_isNewWorker) return;

  m_isNewWorker = false;

  auto oldPipeline        = m_sdfrMaterial->pipeline;
  auto oldPipelineLayout  = m_sdfrMaterial->pipelineLayout;

  m_pipelineHelper.waitForWorkerToFinish();

  m_pipelineHelper.swapSDFRPipelines(
    m_sdfrMaterial,
    m_newSDFRMaterial
  );

  /**
   * @note this waits for the graphics queue/fences to become idle
   * and signals when the existing command buffers finished
   * their execution.
   *
   * Although it may stall the pipeline, that is the only way
   * Qt Vulkan seems to expose in this case. Otherwise, I could use
   * vkGetFenceStatus and pass the Fence to check every frame,
   * which is not available through Qt Vulkan public API.
   *
   * Since the shader compilation is expected to be a one-off
   * design session for the user, than a game realtime/interactive
   * session, therefore the speed is not expected to be
   * very high.
   */

  const auto &queue = m_vkWindow->graphicsQueue();
  const auto &result = m_deviceFuncs->vkQueueWaitIdle(queue);

  if(result == VK_SUCCESS)
  {
    m_pipelineHelper.destroyShaderModule(m_newSDFRMaterial->fragmentShader);
    m_pipelineHelper.destroyDescriptorSetLayout(m_newSDFRMaterial->descSetLayouts);
    m_pipelineHelper.destroyDescriptorPool(m_newSDFRMaterial->descPool);

    m_pipelineHelper.destroyPipelineLayout(oldPipelineLayout);
    m_pipelineHelper.destroyPipeline(oldPipeline);
  }
}
