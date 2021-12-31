#pragma once

#include "Types.hpp"

namespace sdfRay4d
{
  using namespace vk;

  /**
   * @note
   * Vulkan uses PipelineStateObjects
   * similar to D3D 12 \n\n
   * PSOs contain all the states of the pipeline in one object,
   * than a huge state machine like in OpenGL. It's a contract between
   * the application and the driver. \n\n
   * \b Vulkan: \n
   * https://learning.oreilly.com/library/view/learning-vulkan/9781786469809/ch08s05.html#ch08lvl4sec2 \n\n
   * \b D3D12: \n
   * - https://docs.microsoft.com/en-us/windows/win32/direct3d12/pipelines-and-shaders-with-directx-12 \n
   * - https://docs.microsoft.com/en-us/windows/win32/direct3d12/managing-graphics-pipeline-state-in-direct3d-12 \n
   * - https://logins.github.io/graphics/2020/04/12/DX12PipelineStateObject.html
   */
  struct PSO
  {
    pipeline::DynamicInfo       dynamicState;

    // input rate
    pipeline::VertexInputInfo   vertexInputState;

    // assembles the vertex data into primitive's topology
    pipeline::InputAssemblyInfo inputAssemblyState;

    // e.g. polygon fill mode, front facing, culling mode
    pipeline::RasterizationInfo rasterizationState;

    /**
     * @note the blending factor and operation between
     * the source and destination fragments
     */
    pipeline::ColorBlendInfo    colorBlendState;

    // viewports, scissors, and dimensions
    pipeline::ViewportInfo      viewportState;

    // depth/stencil operation
    pipeline::DepthStencilInfo  depthStencilState;

    /**
     * @note controls the samples to be used in pixel depiction
     * during the rasterization for anti-aliasing purposes
     */
    pipeline::MultisampleInfo   multisampleState;

    /**
     * @note Below are somewhat POD containers to store data and avoid
     * having data pointers from going out of scope while trying
     * to access memory between different methods.
     */

    using DynamicStateList      = std::vector<DynamicState>;
    using VertexBindingDescList = std::vector<VertexBindingDesc>;
    using VertexAttrDescList    = std::vector<VertexAttrDesc>;

    // pipeline::DynamicInfo.pDynamicStates is a pointer
    // => needs to persist data in scope via struct
    DynamicStateList                dynamicStates;

    // pipeline::VertexInputInfo.pVertexBindingDescriptions is pointer
    // => needs to persist data in scope via struct
    VertexBindingDescList           vertexBindingDesc;

    // pipeline::VertexInputInfo.pVertexAttributeDescriptions is pointer
    // => needs to persist data in scope via struct
    VertexAttrDescList              vertexAttrDesc;

    // pipeline::ColorBlendInfo.pAttachments is a pointer
    // => needs to persist data in scope via struct
    pipeline::ColorBlendAttachment  colorBlendAttachment;
  };
}
