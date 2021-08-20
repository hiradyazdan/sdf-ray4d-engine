/*****************************************************
 * Partial Class: Pipeline (Ctor & General)
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
 * - create_descriptor_set_helpers.cpp
 * - create_pipeline_helpers.cpp
 * - create_pipeline_workers.cpp
 * - destroy_helpers.cpp
 * - framebuffer_helpers.cpp
 * - init_pso_helpers.cpp
 *****************************************************/

#include "Pipeline.hpp"

using namespace sdfRay4d;

/**
 *
 * @brief initializes the PipelineHelper members
 *
 * @note
 * this replaces the constructor as cannot use a default constructor
 * to initialize device and deviceFuncs members, because they're only initialized
 * after Renderer constructor and within QVulkanRenderer's initResources().
 *
 * @note
 * As attachments is populated later in the frame loop it should
 * be reference, so that its reference value gets used here directly
 * and as they could be multiple attachments/image views later on,
 * it should be either a vector of references which is not directly
 * available in C++ except by using std::reference_wrapper or just pass
 * the ImageView handle as itself is a handle/reference type
 *
 *
 * @param[in] _device
 * @param[in] _deviceFuncs
 * @param[in] _sampleCountFlags
 * @param[in] _defaultRenderPass
 * @param[in] _attachments
 * @param[in] _swapChainImageSize
 * @param[in] _useDefaultRenderPass
 */
void PipelineHelper::init(
  const Device &_device,
  QVulkanDeviceFunctions *_deviceFuncs,
  const SampleCountFlags &_sampleCountFlags,
  const RenderPass &_defaultRenderPass,
  texture::ImageView *_attachments
)
{
  m_device = _device;
  m_deviceFuncs = _deviceFuncs;
  m_sampleCountFlags = _sampleCountFlags;
  m_renderPassHelper = RenderPassHelper(
    _device, _deviceFuncs,
    _sampleCountFlags,
    _defaultRenderPass
  );
  m_renderPass = _defaultRenderPass;
  m_attachments = _attachments; // framebuffer attachments
}

RenderPass &PipelineHelper::getRenderPass(bool _useDefault)
{
  return _useDefault
  ? m_renderPass
  : m_renderPassHelper.getRenderPass();
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
