#pragma once

#include <QVulkanFunctions>

namespace sdfRay4d::vk
{
  using PhysicalDeviceLimits = VkPhysicalDeviceLimits;
  using Buffer = VkBuffer;
  using BufferInfo = VkBufferCreateInfo;
  using Result = VkResult;
  using MemReq = VkMemoryRequirements;
  using MemAlloc = VkMemoryAllocateInfo;

  using ShaderModule      = VkShaderModule;
  using ShaderStageFlags = VkShaderStageFlagBits;
  using VertexBindingDesc = VkVertexInputBindingDescription;
  using VertexAttrDesc    = VkVertexInputAttributeDescription;
  using DynamicState      = VkDynamicState;
  using Device = VkDevice;
  using Pipeline = VkPipeline;

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

  namespace pipeline
  {
    using Cache = VkPipelineCache;
    using CacheInfo                 = VkPipelineCacheCreateInfo;
    using Layout = VkPipelineLayout;
    using LayoutInfo                = VkPipelineLayoutCreateInfo;
    using ShaderStage           = VkPipelineShaderStageCreateInfo;
    using VertexInputState      = VkPipelineVertexInputStateCreateInfo;
    using GraphicsPipeline      = VkGraphicsPipelineCreateInfo;
    using InputAssembly         = VkPipelineInputAssemblyStateCreateInfo;
    using Viewport              = VkPipelineViewportStateCreateInfo;
    using Rasterization         = VkPipelineRasterizationStateCreateInfo;
    using Multisample           = VkPipelineMultisampleStateCreateInfo;
    using DepthStencil          = VkPipelineDepthStencilStateCreateInfo;
    using ColorBlend            = VkPipelineColorBlendStateCreateInfo;
    using ColorBlendAttachment  = VkPipelineColorBlendAttachmentState;
    using Dynamic               = VkPipelineDynamicStateCreateInfo;
  }
}
