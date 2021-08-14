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
 * - init_pso_helpers.cpp
 *****************************************************/

#include "Pipeline.hpp"

using namespace sdfRay4d;

/**
 *
 * @brief initializes the PipelineHelper members
 * @note this replaces the constructor as we cannot use a default constructor
 * to initialize device and deviceFuncs members, because they're only initialized
 * after Renderer constructor and within QVulkanRenderer's initResources()
 *
 * @param _device
 * @param _deviceFuncs
 * @param _sampleCountFlags
 * @param _defaultRenderPass
 * @param _useDefaultRenderPass
 */
void PipelineHelper::init(
  const Device &_device,
  QVulkanDeviceFunctions *_deviceFuncs,
  const SampleCountFlags &_sampleCountFlags,
  const RenderPass &_defaultRenderPass,
  bool _useDefaultRenderPass
)
{
  m_device = _device;
  m_deviceFuncs = _deviceFuncs;
  m_sampleCountFlags = _sampleCountFlags;
  m_renderPassHelper = RenderPassHelper(
    _device, _deviceFuncs,
    _sampleCountFlags,
    _defaultRenderPass,
    _useDefaultRenderPass
  );
  m_renderPass = _useDefaultRenderPass
    ? _defaultRenderPass
    : m_renderPassHelper.getRenderPass();
}

void PipelineHelper::waitForWorkersToFinish()
{
  for (auto &worker : m_workers)
  {
    worker.waitForFinished();
  }
}
