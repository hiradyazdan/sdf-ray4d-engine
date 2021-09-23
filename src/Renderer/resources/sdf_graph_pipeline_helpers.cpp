/*****************************************************
 * Partial Class: Renderer
 * Members: SDF Graph Material Pipeline Helpers (Private)
 *****************************************************/

#include "Renderer.hpp"

using namespace sdfRay4d;

/**
 * @brief creates and passes a fresh new material
 * to be consumed by the SDF Graph
 * @param[in] _isNew
 * @return MaterialPtr
 */
Renderer::MaterialPtr &Renderer::getSDFRMaterial(bool _isNew)
{
  if(!_isNew) return m_newSDFRMaterial;

  m_newSDFRMaterial = std::make_shared<Mat>(
    m_device,
    m_deviceFuncs
  );

  return m_newSDFRMaterial;
}

void Renderer::createSDFRPipeline()
{
  /**
   * @note at this point, new SDFR Material, should've been created
   * so, will reuse the same new material for every compile
   */
  const auto &newMaterial = getSDFRMaterial();

  initSDFRMaterial(newMaterial);

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

  m_oldPipeline.pipeline  = m_sdfrMaterial->pipeline;
  m_oldPipeline.layout    = m_sdfrMaterial->pipelineLayout;

  m_pipelineHelper.waitForWorkerToFinish();
  m_destroyWorker.waitForFinished();

  m_pipelineHelper.swapSDFRPipelines(
    m_sdfrMaterial,
    m_newSDFRMaterial
  );

  /**
   * @note this waits for the graphics queue/fences to become idle
   * and signals when the existing command buffers finished
   * their execution.
   *
   * @note Although it may stall the pipeline, that is the only way
   * Qt Vulkan seems to expose in this case. Otherwise, I could use
   * vkGetFenceStatus and pass the Fence to check every frame,
   * which is not available through Qt Vulkan public API.
   *
   * @note Since the shader compilation is expected to be a one-off
   * design session for the user, than a game realtime/interactive
   * session, therefore the speed is not expected to be
   * very high.
   */

  const auto &queue = m_vkWindow->graphicsQueue();
  const auto &result = m_deviceFuncs->vkQueueWaitIdle(queue);

  if(result == VK_SUCCESS)
  {
    /**
     * @note this worker helps clearing up all stale/orphaned
     * vulkan objects for auto compile as a follow up for swapWorker.
     * FIXME:
     * But may not clear up some;
     * - if switch back & forth between manual and auto, and then make changes
     *   quite repeatedly and fast.
     * - if multiple objects on auto-compile are loaded and changed quite
     *   repeatedly and fast.
     */
    m_destroyWorker = QtConcurrent::run([this]()
    {
      m_pipelineHelper.destroyShaderModule(m_newSDFRMaterial->fragmentShader);
      m_pipelineHelper.destroyDescriptorSetLayout(m_newSDFRMaterial->descSetLayouts);
      m_pipelineHelper.destroyDescriptorPool(m_newSDFRMaterial->descPool);
      m_pipelineHelper.destroyTexture(m_newSDFRMaterial->texture);

      m_pipelineHelper.destroyPipelineLayout(m_oldPipeline.layout);
      m_pipelineHelper.destroyPipeline(m_oldPipeline.pipeline);
    });
  }
}
