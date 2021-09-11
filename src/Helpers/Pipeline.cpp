/*****************************************************
 * Partial Class: PipelineHelper (Ctor & General)
 * Members: General Functions (Public)
 *
 * This Class is split into partials to categorize
 * and classify the functionality
 * for the purpose of readability/maintainability
 *
 * The partials can be found in the respective
 * directory named as the class name
 *
 * Partials:
 * - create_pipeline_helpers.cpp
 * - create_pipeline_workers.cpp
 * - destroy_helpers.cpp
 * - init_pso_helpers.cpp
 *****************************************************/

#include "Helpers/Pipeline.hpp"

using namespace sdfRay4d::helpers;

/**
 *
 * @brief initializes the PipelineHelper members
 *
 * @note
 * this replaces the constructor as cannot use a default constructor
 * to initialize device and deviceFuncs members, because they're only initialized
 * after Renderer constructor and within QVulkanRenderer's initResources().
 *
 * @param[in] _device
 * @param[in] _deviceFuncs
 * @param[in] _sampleCountFlags
 * @param[in] _defaultRenderPass
 *
 */
void PipelineHelper::initHelpers(
  const Device &_device,
  QVulkanDeviceFunctions *_deviceFuncs,
  const SampleCountFlags &_sampleCountFlags,
  const RenderPass &_defaultRenderPass
)
{
  m_device            = _device;
  m_deviceFuncs       = _deviceFuncs;
  m_sampleCountFlags  = _sampleCountFlags;

  m_renderPassHelper  = RenderPassHelper(
    m_device, m_deviceFuncs,
    m_sampleCountFlags,
    _defaultRenderPass
  );

  descriptor          = DescriptorHelper(m_device, m_deviceFuncs);
  command             = CommandHelper(m_device, m_deviceFuncs);
}

/**
 *
 * @param[in] _fbAttachments framebuffer attachments
 */
void PipelineHelper::initSwapChainHelpers(
  const texture::ImageView &_fbAttachments
)
{
  if(m_renderPassHelper.getFramebufferAttachments() == _fbAttachments) return;

  m_renderPassHelper.setFramebufferAttachments(_fbAttachments);
  buffer = BufferHelper(m_device, m_deviceFuncs);
  command.setRenderPassHelper(m_renderPassHelper);
}

/**
 *
 * @param[in] _useDefault
 * @return RenderPass instance
 */
RenderPass &PipelineHelper::getRenderPass(bool _useDefault)
{
  return m_renderPassHelper.getRenderPass(_useDefault);
}

void PipelineHelper::waitForWorkersToFinish()
{
  for (auto &worker : m_workers)
  {
    worker.waitForFinished();
  }
}

/**
 * TODO: this can be modified to request for
 * specific worker than implicitly request for
 * a designated worker which was previously
 * assigned to exclusiveWorker member. But currently,
 * the use case is limited to SDF Graph pipeline
 * creation.
 */
void PipelineHelper::waitForWorkerToFinish()
{
  m_exclusiveWorker.waitForFinished();
}

bool PipelineHelper::isWorkerFinished()
{
  return m_exclusiveWorker.isFinished();
}
