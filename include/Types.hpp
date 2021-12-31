#pragma once

#include <QVulkanFunctions>

#include "pch.hpp"
#include "_constants.hpp"

namespace sdfRay4d
{
  namespace qt
  {}

  namespace vk
  {
    using PhysicalDeviceLimits  = VkPhysicalDeviceLimits;
    using Result                = VkResult;
    using Format                = VkFormat;

    using VertexBindingDesc     = VkVertexInputBindingDescription;
    using VertexAttrDesc        = VkVertexInputAttributeDescription;
    using DynamicState          = VkDynamicState;

    using CmdPool               = VkCommandPool;
    using CmdBuffer             = VkCommandBuffer;

    using ClearColor            = VkClearColorValue;
    using ClearDepthStencil     = VkClearDepthStencilValue;
    using Clear                 = VkClearValue;
    using Viewport              = VkViewport;
    using Rect2D                = VkRect2D;
    using PushConstantRange     = VkPushConstantRange;
    using SampleCountFlags      = VkSampleCountFlagBits;

    using AttachmentDesc        = VkAttachmentDescription;
    using AttachmentRef         = VkAttachmentReference;
    using SubpassDesc           = VkSubpassDescription;

    namespace renderpass
    {
      using RenderPass  = VkRenderPass;

      using Info        = VkRenderPassCreateInfo;
      using BeginInfo   = VkRenderPassBeginInfo;

      // RenderPass StructureType
      struct StructureType
      {
        static constexpr const VkStructureType RENDER_PASS_INFO       = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        static constexpr const VkStructureType RENDER_PASS_BEGIN_INFO = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
      };
    }

    namespace framebuffer
    {
      using Framebuffer = VkFramebuffer;

      using Info        = VkFramebufferCreateInfo;

      // Framebuffer StructureType
      struct StructureType
      {
        static constexpr const VkStructureType FRAMEBUFFER_INFO = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
      };
    }

    namespace texture
    {
      using Image               = VkImage;
      using ImageInfo           = VkImageCreateInfo;
      using ImageUsage          = VkImageUsageFlags;
      using ImageView           = VkImageView;
      using ImageViewInfo       = VkImageViewCreateInfo;
      using ImageAspect         = VkImageAspectFlags;
      using ImageLayout         = VkImageLayout;
      using ImageMemoryBarrier  = VkImageMemoryBarrier;

      using AccessFlags         = VkAccessFlags;

      using Sampler             = VkSampler;
      using SamplerInfo         = VkSamplerCreateInfo;

      // Texture StructureType
      struct StructureType
      {
        static constexpr const VkStructureType IMAGE_INFO             = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        static constexpr const VkStructureType IMAGE_VIEW_INFO        = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        static constexpr const VkStructureType IMAGE_MEMORY_BARRIER   = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;

        static constexpr const VkStructureType SAMPLER_INFO           = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
      };
    }

    namespace buffer
    {
      using Buffer      = VkBuffer;

      using Info        = VkBufferCreateInfo;
      using UsageFlags  = VkBufferUsageFlags;

      // Buffer StructureType
      struct StructureType
      {
        static constexpr const VkStructureType BUFFER_INFO = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
      };
    }

    namespace memory
    {
      using Reqs      = VkMemoryRequirements;
      using AllocInfo = VkMemoryAllocateInfo;

      // Memory StructureType
      struct StructureType
      {
        static constexpr const VkStructureType MEMORY_ALLOC_INFO = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
      };
    }

    namespace device
    {
      using Device  = VkDevice;

      using Size    = VkDeviceSize;
      using Memory  = VkDeviceMemory;
    }

    namespace descriptor
    {
      using BufferInfo    = VkDescriptorBufferInfo;
      using ImageInfo     = VkDescriptorImageInfo;
      using Set           = VkDescriptorSet;

      using Pool          = VkDescriptorPool;
      using PoolInfo      = VkDescriptorPoolCreateInfo;
      using PoolFlags     = VkDescriptorPoolCreateFlags;
      using PoolSize      = VkDescriptorPoolSize;

      using Layout        = VkDescriptorSetLayout;
      using LayoutInfo    = VkDescriptorSetLayoutCreateInfo;
      using LayoutBinding = VkDescriptorSetLayoutBinding;

      using AllocInfo     = VkDescriptorSetAllocateInfo;
      using Write         = VkWriteDescriptorSet;

      // Descriptor StructureType
      struct StructureType
      {
        static constexpr const VkStructureType POOL_INFO        = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;

        static constexpr const VkStructureType SET_ALLOC_INFO   = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        static constexpr const VkStructureType SET_LAYOUT_INFO  = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;

        static constexpr const VkStructureType WRITE_SET        = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
      };
    }

    namespace shader
    {
      using Module      = VkShaderModule;
      using Info        = VkShaderModuleCreateInfo;
      using StageFlag   = VkShaderStageFlagBits;
      using StageFlags  = VkShaderStageFlags;

      // Shader StructureType
      struct StructureType
      {
        static constexpr const VkStructureType SHADER_MODULE_INFO  = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
      };
    }

    namespace pipeline
    {
      using Pipeline              = VkPipeline;

      using Cache                 = VkPipelineCache;
      using CacheInfo             = VkPipelineCacheCreateInfo;

      using Layout                = VkPipelineLayout;
      using LayoutInfo            = VkPipelineLayoutCreateInfo;

      using ShaderStageInfo       = VkPipelineShaderStageCreateInfo;

      // PSOs
      using VertexInputInfo       = VkPipelineVertexInputStateCreateInfo;
      using InputAssemblyInfo     = VkPipelineInputAssemblyStateCreateInfo;
      using ViewportInfo          = VkPipelineViewportStateCreateInfo;
      using RasterizationInfo     = VkPipelineRasterizationStateCreateInfo;
      using MultisampleInfo       = VkPipelineMultisampleStateCreateInfo;
      using DepthStencilInfo      = VkPipelineDepthStencilStateCreateInfo;
      using ColorBlendInfo        = VkPipelineColorBlendStateCreateInfo;
      using DynamicInfo           = VkPipelineDynamicStateCreateInfo;

      using ColorBlendAttachment  = VkPipelineColorBlendAttachmentState;

      using StageFlags            = VkPipelineStageFlags;

      // Pipelines Info
      using GraphicsPipelineInfo  = VkGraphicsPipelineCreateInfo;
      using ComputePipelineInfo   = VkComputePipelineCreateInfo;

      // Pipeline StructureType
      struct StructureType
      {
        // Pipeline States Infos
        static constexpr const VkStructureType SHADER_STAGE_INFO      = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        static constexpr const VkStructureType VERTEX_INPUT_INFO      = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        static constexpr const VkStructureType INPUT_ASSEMBLY_INFO    = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        static constexpr const VkStructureType RASTERIZATION_INFO     = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        static constexpr const VkStructureType VIEWPORT_INFO          = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        static constexpr const VkStructureType MULTISAMPLE_INFO       = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        static constexpr const VkStructureType DEPTH_STENCIL_INFO     = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
        static constexpr const VkStructureType COLOR_BLEND_INFO       = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        static constexpr const VkStructureType DYNAMIC_INFO           = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;

        static constexpr const VkStructureType LAYOUT_INFO            = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        static constexpr const VkStructureType CACHE_INFO             = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;

        static constexpr const VkStructureType GRAPHICS_PIPELINE_INFO = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        static constexpr const VkStructureType COMPUTE_PIPELINE_INFO  = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
      };
    }
  }
}
