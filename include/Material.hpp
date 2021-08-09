#pragma once

#include "Shader.hpp"
#include "PSO.hpp"

namespace sdfRay4d
{
  /**
   * Material should be an independent struct
   * to make it possible to be adopted by either
   * SDF Graph or another rendering approach
   */
  struct Material
  {
    using ShaderStageInfoList = std::vector<pipeline::ShaderStageInfo>;

    Material(
      Device &_device,
      QVulkanDeviceFunctions *_deviceFuncs
    ) :
      vertexShader(_device, _deviceFuncs)
      , fragmentShader(_device, _deviceFuncs)
      , computeShader(_device, _deviceFuncs)
    {}

//    device::Size        vertUniSize;
//    device::Size        fragUniSize;
//    device::Size        uniMemStartOffset;

    Shader              vertexShader;
    Shader              fragmentShader;
    Shader              computeShader;

    descriptor::Pool    descPool = VK_NULL_HANDLE;
    descriptor::Layout  descSetLayout = VK_NULL_HANDLE;
    descriptor::Set     descSet = VK_NULL_HANDLE;

    pipeline::Layout    pipelineLayout = VK_NULL_HANDLE;
    PSO                 pso;
    ShaderStageInfoList shaderStages;

    Pipeline            pipeline = VK_NULL_HANDLE;
  };
}
