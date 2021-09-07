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
   *
   * @note CTAD (Class Template Argument Deduction) is supported on C++17 with
   * - gcc 8.0+
   * - clang 7.0+
   * - msvc 2017+ (only with std:c++17 and std:c++latest compiler options)
   *
   * https://devblogs.microsoft.com/cppblog/how-to-use-class-template-argument-deduction
   *
   * TODO:
   * - make Material a pure POD by setting device and device functions for respective
   *   components outside their ctors with generic/global initializer function
   * - abstract Material components with similar functionality into reusable wrapper structs
   */
  template<typename TConst = float>
  struct Material
  {
    using ShaderStageInfoList = std::vector<pipeline::ShaderStageInfo>;
    using LayoutBindingList   = std::vector<descriptor::LayoutBinding>;
    using DescLayoutList      = std::vector<descriptor::Layout>;
    using DescSettList        = std::vector<descriptor::Set>;

    // Shader
    Shader                      vertexShader;
    Shader                      fragmentShader;
    Shader                      computeShader;

    // Texture
    Texture                     texture;

    // Buffer
    Buffer                      buffer = VK_NULL_HANDLE;
    memory::Reqs                memReq = {};

    // Descriptor
    descriptor::Pool            descPool = VK_NULL_HANDLE;
    LayoutBindingList           layoutBindings;
    DescLayoutList              descSetLayouts;
    DescSettList                descSets;
    uint32_t                    descSetLayoutCount = 0;
    uint32_t                    dynamicDescCount = 0;

    // PushConstant
    uint32_t                    pushConstantRangeCount = 0;
    PushConstantRange           pushConstantRange = {};
    std::vector<TConst>         pushConstants = {};

    // PSO
    PSO                         pso;

    // Pipeline
    ShaderStageInfoList         shaderStages;
    pipeline::StageFlags        sourceStage = {};
    pipeline::StageFlags        destinationStage = {};
    pipeline::Layout            pipelineLayout = VK_NULL_HANDLE;
    Pipeline                    pipeline = VK_NULL_HANDLE;

    // RenderPass
    RenderPass                  renderPass = VK_NULL_HANDLE;
    Framebuffer                 framebuffer = VK_NULL_HANDLE;

    device::Size                vertUniSize = 0;
    device::Size                fragUniSize = 0;
    device::Size                uniMemStartOffset = 0;

    uint32_t                    vertexCount = 0;

    bool                        isHotSwappable = false;
    bool                        isDefault = true;

    /**
     * @brief Material struct holds pipeline data throughout their
     * entire lifecycle
     *
     * @note They're similar to PODs with the exception of this
     * ctor that is needed only to pass device functions
     * for creating shaders and textures
     *
     * @param[in] _device
     * @param[in] _deviceFuncs
     */
    Material(
      Device &_device,
      QVulkanDeviceFunctions *_deviceFuncs
    ) :
      vertexShader(_device, _deviceFuncs)
    , fragmentShader(_device, _deviceFuncs)
    , computeShader(_device, _deviceFuncs)
    , texture(_device, _deviceFuncs)
    {};

    /**
     * @brief creates push constants with range data
     * @note
     * There's no need to allow
     * for creating more than one
     * push constant for each pipeline
     * as the max size allowed is only 128 bytes
     *
     * @param[in] _offset push constants range offset
     * @param[in] _size push constants range size
     * @param[in] _pushConstants push constants data
     * @param[in] _stageFlags one or more shader stage flag bits
     */
    void createPushConstants(
      uint32_t _offset,
      uint32_t _size,
      std::vector<TConst> _pushConstants,
      shader::StageFlags _stages = VK_SHADER_STAGE_FRAGMENT_BIT
    )
    {
      pushConstantRangeCount = 1;

      pushConstantRange.stageFlags = _stages;
      pushConstantRange.offset = _offset;
      pushConstantRange.size = _size;

      pushConstants = _pushConstants;
    }
  };
}
