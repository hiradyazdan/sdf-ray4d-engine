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
    using LayoutBindingList   = std::vector<descriptor::LayoutBinding>;
    using DescLayoutList      = std::vector<descriptor::Layout>;
    using DescSettList        = std::vector<descriptor::Set>;

    Shader                      vertexShader;
    Shader                      fragmentShader;
    Shader                      computeShader;

    Texture                     texture;

    descriptor::Pool            descPool = VK_NULL_HANDLE;
    LayoutBindingList           layoutBindings;
    DescLayoutList              descSetLayouts;
    DescSettList                descSets;
    uint32_t                    descSetLayoutCount = 0;

    PushConstantRange           pushConstantRange = {};
    uint32_t                    pushConstantRangeCount = 0;

    PSO                         pso;

    pipeline::Layout            pipelineLayout = VK_NULL_HANDLE;
    Pipeline                    pipeline = VK_NULL_HANDLE;
    ShaderStageInfoList         shaderStages;
    RenderPass                  renderPass = VK_NULL_HANDLE;

    device::Size                vertUniSize = 0;
    device::Size                fragUniSize = 0;
    device::Size                uniMemStartOffset = 0;

    bool                        isHotSwappable = false;

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
