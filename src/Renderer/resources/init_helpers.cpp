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

  m_pipelineHelper.init(
    m_device,
    m_deviceFuncs,
    m_isMSAA ? m_vkWindow->sampleCountFlagBits() : VK_SAMPLE_COUNT_1_BIT,
    m_vkWindow->defaultRenderPass(),
    &m_imageView
  );
}

void Renderer::initMaterials()
{
  auto *deviceLimits = &m_vkWindow->physicalDeviceProperties()->limits;

  initActorMaterial(deviceLimits);
  initSDFRMaterial(deviceLimits);

  m_materials = {
    m_actorMaterial,
    m_sdfrMaterial
  };
}

void Renderer::initShaders()
{
  initActorShaders();
  initSDFRShaders();
}
