/*****************************************************
 * Struct: Material (General)
 * Members: General Functions (Public)
 * Partials: None
 *****************************************************/

#include "Material.hpp"

using namespace sdfRay4d;

Material::Material(
  Device &_device,
  QVulkanDeviceFunctions *_deviceFuncs
) :
  vertexShader(_device, _deviceFuncs)
  , fragmentShader(_device, _deviceFuncs)
  , computeShader(_device, _deviceFuncs)
  , depthTexture(_device, _deviceFuncs)
{}

void Material::createPushConstantRange(
  uint32_t _offset,
  uint32_t _size
)
{
  /**
   * @note
   * There's no need to allow
   * for creating more than one
   * push constant for each pipeline
   * as the max size allowed is only 128 bytes
   */

  PushConstantRange pushConstant;

  pushConstant.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
  pushConstant.offset = _offset;
  pushConstant.size = _size;

  pushConstantRange = pushConstant;
  pushConstantRangeCount = 1;
}
