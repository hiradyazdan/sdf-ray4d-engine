#pragma once

#include "Shader.hpp"
#include "Texture.hpp"
#include "PSO.hpp"

namespace sdfRay4d
{
  /**
   * @struct Material
   * @brief A wrapper for components of each pipeline
   *
   * @note Material should be an independent struct
   * to make it possible to be adopted by either
   * SDF Graph or another rendering mechanism
   */
  struct Material
  {
    using ShaderStageInfoList = std::vector<pipeline::ShaderStageInfo>;

    device::Size                vertUniSize = 0;
    device::Size                fragUniSize = 0;
    device::Size                uniMemStartOffset = 0;

    Shader                      vertexShader;
    Shader                      fragmentShader;
    Shader                      computeShader;

    Texture                     depthTexture;

    uint32_t                    descSetLayoutCount = 0;
    uint32_t                    pushConstantRangeCount = 0;

    descriptor::Pool            descPool = VK_NULL_HANDLE;
    descriptor::LayoutBinding   layoutBindings[2];
    descriptor::Layout          descSetLayout = VK_NULL_HANDLE;
    descriptor::Set             descSet = VK_NULL_HANDLE;

    PushConstantRange           pushConstantRange = {};
    pipeline::Layout            pipelineLayout = VK_NULL_HANDLE;
    PSO                         pso;
    ShaderStageInfoList         shaderStages;

    Pipeline                    pipeline = VK_NULL_HANDLE;

    Material(
      Device &_device,
      QVulkanDeviceFunctions *_deviceFuncs
    );

    void createPushConstantRange(
      uint32_t _offset,
      uint32_t _size
    );
  };
}
