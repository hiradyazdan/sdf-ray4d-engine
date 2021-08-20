/*****************************************************
 * Struct: Material (General)
 * Members: General Functions (Public)
 * Partials: None
 *****************************************************/

#include "Material.hpp"

using namespace sdfRay4d;

/**
 * @brief Material struct holds pipeline data throughout their
 * entire lifecycle
 *
 * @note They're similar to PODs with the exception of this
 * ctor that is needed only to pass device functions
 * for creating shaders and textures
 *
 *
 * @param[in] _device
 * @param[in] _deviceFuncs
 */
Material::Material(
  Device &_device,
  QVulkanDeviceFunctions *_deviceFuncs
) :
  vertexShader(_device, _deviceFuncs)
  , fragmentShader(_device, _deviceFuncs)
  , computeShader(_device, _deviceFuncs)
  , texture(_device, _deviceFuncs)
{}

/**
 *
 * @note
 * There's no need to allow
 * for creating more than one
 * push constant for each pipeline
 * as the max size allowed is only 128 bytes
 *
 * @param[in] _offset
 * @param[in] _size
 */
void Material::createPushConstantRange(
  uint32_t _offset,
  uint32_t _size
)
{
  PushConstantRange pushConstant;

  pushConstant.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
  pushConstant.offset = _offset;
  pushConstant.size = _size;

  pushConstantRange = pushConstant;
  pushConstantRangeCount = 1;
}
