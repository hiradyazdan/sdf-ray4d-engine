/*****************************************************
 * Partial Class: Renderer
 * Members: Init Resources Helpers (Private)
 *****************************************************/

#include "Renderer.hpp"

using namespace sdfRay4d;

void Renderer::initVkFunctions()
{
  auto &&vkInstance = m_vkWindow->vulkanInstance();
  m_device          = m_vkWindow->device();
  m_deviceFuncs     = vkInstance->deviceFunctions(m_device);

  m_pipelineHelper.initHelpers(
    m_device,
    m_deviceFuncs,
    m_isMSAA ? m_vkWindow->sampleCountFlagBits() : VK_SAMPLE_COUNT_1_BIT,
    m_vkWindow->defaultRenderPass()
  );
}

void Renderer::initMaterials()
{
  initDepthMaterial();
  initActorMaterial();
  initSDFRMaterial();
}

void Renderer::initShaders()
{
  initDepthShaders();
  initActorShaders();
  initSDFRShaders();
}

const VkPhysicalDeviceLimits *Renderer::getDeviceLimits() const
{
  return &m_vkWindow->physicalDeviceProperties()->limits;
}

device::Size Renderer::setDynamicOffsetAlignment(
  device::Size _offset
)
{
  const auto &minUBOAlignment = getDeviceLimits()->minUniformBufferOffsetAlignment;

  return (_offset + minUBOAlignment - 1) & ~(minUBOAlignment - 1);
}
