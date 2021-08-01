#pragma once

#include <QVulkanFunctions>

namespace sdfRay4d::vk
{
  using PhysicalDeviceLimits = VkPhysicalDeviceLimits;
  using Buffer = VkBuffer;
  using BufferInfo = VkBufferCreateInfo;
  using Result = VkResult;

  using VertexBindingDesc = VkVertexInputBindingDescription;
  using VertexAttrDesc    = VkVertexInputAttributeDescription;
  using DynamicState      = VkDynamicState;
  using Device = VkDevice;
  using Pipeline = VkPipeline;

  using CmdBuffer = VkCommandBuffer;
  using ClearColor = VkClearColorValue;
  using ClearDepthStencil = VkClearDepthStencilValue;
  using Clear = VkClearValue;
  using RenderPassBeginInfo = VkRenderPassBeginInfo;
  using Viewport = VkViewport;
  using Rect2D = VkRect2D;
  using PushConstantRange = VkPushConstantRange;

  namespace memory
  {
    using Reqs = VkMemoryRequirements;
    using AllocInfo = VkMemoryAllocateInfo;
  }

  namespace device
  {
    using Size = VkDeviceSize;
    using Memory = VkDeviceMemory;
  }

  namespace descriptor
  {
    using BufferInfo = VkDescriptorBufferInfo;
    using Set = VkDescriptorSet;

    using Pool = VkDescriptorPool;
    using PoolInfo = VkDescriptorPoolCreateInfo;
    using PoolSize = VkDescriptorPoolSize;

    using Layout = VkDescriptorSetLayout;
    using LayoutInfo = VkDescriptorSetLayoutCreateInfo;
    using LayoutBinding = VkDescriptorSetLayoutBinding;

    using AllocInfo = VkDescriptorSetAllocateInfo;
    using Write = VkWriteDescriptorSet;
  }

  namespace shader
  {
    using Module      = VkShaderModule;
    using Info        = VkShaderModuleCreateInfo;
    using StageFlags  = VkShaderStageFlagBits;
  }

  namespace pipeline
  {
    using Cache = VkPipelineCache;
    using CacheInfo                 = VkPipelineCacheCreateInfo;
    using Layout = VkPipelineLayout;

    using LayoutInfo                = VkPipelineLayoutCreateInfo;
    using ShaderStageInfo           = VkPipelineShaderStageCreateInfo;
    using VertexInputInfo      = VkPipelineVertexInputStateCreateInfo;
    using GraphicsPipelineInfo      = VkGraphicsPipelineCreateInfo;
    using InputAssemblyInfo         = VkPipelineInputAssemblyStateCreateInfo;
    using ViewportInfo          = VkPipelineViewportStateCreateInfo;
    using RasterizationInfo         = VkPipelineRasterizationStateCreateInfo;
    using MultisampleInfo           = VkPipelineMultisampleStateCreateInfo;
    using DepthStencilInfo          = VkPipelineDepthStencilStateCreateInfo;
    using ColorBlendInfo            = VkPipelineColorBlendStateCreateInfo;
    using DynamicInfo          = VkPipelineDynamicStateCreateInfo;

    using ColorBlendAttachment  = VkPipelineColorBlendAttachmentState;
  }

//  typedef enum StructureType
//  {
//    // Pipeline States Infos
//
//    PIPELINE_SHADER_STAGE_INFO = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
//    PIPELINE_LAYOUT_INFO = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
//    PIPELINE_VERTEX_INPUT_INFO = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
//    PIPELINE_INPUT_ASSEMBLY_INFO = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
//    PIPELINE_RASTERIZATION_INFO = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
//    PIPELINE_VIEWPORT_INFO = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
//    PIPELINE_MULTISAMPLE_INFO = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
//    PIPELINE_DEPTH_STENCIL_INFO = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO,
//    PIPELINE_COLOR_BLEND_INFO = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
//    PIPELINE_DYNAMIC_INFO = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
//
//    GRAPHICS_PIPELINE_INFO = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
//
//    BUFFER_INFO = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
//  } StructureType;
}