#pragma once

#include <QVulkanFunctions>

#include "pch.hpp"
#include "_constants.hpp"

namespace sdfRay4d
{
  /**
   * @struct NOP
   * @brief (no-op) to be used as a super struct for any struct that
   * represent compile-time type containers/namespaces than instantiable
   * structures by removing their default constructors
   */
  struct NOP
  {
    NOP() = delete;
    NOP(const NOP&) = delete; // non-copyable
    NOP& operator=(const NOP&) = delete; // non-copyable
  };

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

    using Clear                 = VkClearValue;
    using ClearColor            = VkClearColorValue;
    using ClearDepthStencil     = VkClearDepthStencilValue;
    using Viewport              = VkViewport;
    using Rect2D                = VkRect2D;
    using PushConstantRange     = VkPushConstantRange;

    namespace renderpass
    {
      using RenderPass      = VkRenderPass;

      using Info            = VkRenderPassCreateInfo;
      using BeginInfo       = VkRenderPassBeginInfo;

      using SubpassDesc     = VkSubpassDescription;

      using AttachmentDesc  = VkAttachmentDescription;
      using AttachmentRef   = VkAttachmentReference;

      // RenderPass StructureType
      struct StructureType : NOP
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
      struct StructureType : NOP
      {
        static constexpr const VkStructureType FRAMEBUFFER_INFO = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
      };
    }

    namespace image
    {
      using Image         = VkImage;

      using Info          = VkImageCreateInfo;
      using Usage         = VkImageUsageFlags;

      using View          = VkImageView;
      using ViewInfo      = VkImageViewCreateInfo;

      using Aspect        = VkImageAspectFlags;
      using Layout        = VkImageLayout;
      using MemBarrier    = VkImageMemoryBarrier;

      using Sampler             = VkSampler;
      using SamplerInfo         = VkSamplerCreateInfo;

      using SampleCountFlagBits = VkSampleCountFlagBits;
      using SampleCountFlags    = VkSampleCountFlags;

      // Image StructureType
      struct StructureType : NOP
      {
        static constexpr const VkStructureType IMAGE_INFO = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        static constexpr const VkStructureType IMAGE_VIEW_INFO = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        static constexpr const VkStructureType IMAGE_MEMORY_BARRIER = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;

        static constexpr const VkStructureType SAMPLER_INFO           = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
      };
    }

    namespace buffer
    {
      using Buffer      = VkBuffer;

      using Info        = VkBufferCreateInfo;
      using UsageFlags  = VkBufferUsageFlags;

      // Buffer StructureType
      struct StructureType : NOP
      {
        static constexpr const VkStructureType BUFFER_INFO = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
      };
    }

    namespace command
    {
      using CmdPool   = VkCommandPool;
      using CmdBuffer = VkCommandBuffer;
    }

    namespace memory
    {
      using Reqs        = VkMemoryRequirements;
      using AllocInfo   = VkMemoryAllocateInfo;

      using AccessFlags = VkAccessFlags;

      // Memory StructureType
      struct StructureType : NOP
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
      struct StructureType : NOP
      {
        static constexpr const VkStructureType POOL_INFO        = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;

        static constexpr const VkStructureType SET_ALLOC_INFO   = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        static constexpr const VkStructureType SET_LAYOUT_INFO  = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;

        static constexpr const VkStructureType WRITE_SET        = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
      };
    }

    namespace shader
    {
      using Module        = VkShaderModule;
      using Info          = VkShaderModuleCreateInfo;
      using StageFlagBits = VkShaderStageFlagBits;
      using StageFlags    = VkShaderStageFlags;

      // Shader StructureType
      struct StructureType : NOP
      {
        static constexpr const VkStructureType SHADER_MODULE_INFO  = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
      };

      struct StageFlag : NOP
      {
        static constexpr const StageFlagBits VERTEX                   = VK_SHADER_STAGE_VERTEX_BIT;
        static constexpr const StageFlagBits FRAGMENT                 = VK_SHADER_STAGE_FRAGMENT_BIT;
        static constexpr const StageFlagBits COMPUTE                  = VK_SHADER_STAGE_COMPUTE_BIT;
        static constexpr const StageFlagBits GEOMETRY                 = VK_SHADER_STAGE_GEOMETRY_BIT;
        static constexpr const StageFlagBits TESSELLATION_CONTROL     = VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
        static constexpr const StageFlagBits TESSELLATION_EVALUATION  = VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;
        static constexpr const StageFlagBits RAYGEN_KHR               = VK_SHADER_STAGE_RAYGEN_BIT_KHR;
        static constexpr const StageFlagBits ANY_HIT_KHR              = VK_SHADER_STAGE_ANY_HIT_BIT_KHR;
        static constexpr const StageFlagBits CLOSEST_HIT_KHR          = VK_SHADER_STAGE_CLOSEST_HIT_BIT_KHR;
        static constexpr const StageFlagBits MISS_KHR                 = VK_SHADER_STAGE_MISS_BIT_KHR;
        static constexpr const StageFlagBits INTERSECTION_KHR         = VK_SHADER_STAGE_INTERSECTION_BIT_KHR;
        static constexpr const StageFlagBits CALLABLE_KHR             = VK_SHADER_STAGE_CALLABLE_BIT_KHR;
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
      struct StructureType : NOP
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
