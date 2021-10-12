#pragma once

#include <QtConcurrentRun>
#include <QSize>

#include "BaseHelper.hpp"

#include "Descriptor.hpp"
#include "Buffer.hpp"
#include "Command.hpp"
#include "Framebuffer.hpp"
#include "RenderPass.hpp"

namespace sdfRay4d::vkHelpers
{
  /**
   * @class PipelineHelper
   * @brief
   *
   * @note as Material is relatively a big struct with high level
   * of usage and importance, throughout the entire application
   * lifecycle, all functions that use it as a parameter, designated
   * the parameter as a const reference (&), to use the same memory
   * address and keep the consistency of the material data intact.
   *
   */
  class PipelineHelper : protected BaseHelper
  {
    using ShaderStageInfoList = std::vector<pipeline::ShaderStageInfo>;
    using DescLayoutList      = std::vector<descriptor::Layout>;
    using ImageViewList       = std::vector<texture::ImageView>;

    public:
      DescriptorHelper descriptor;
      BufferHelper buffer;
      CommandHelper command;

    public:
      void initHelpers(
        const Device &_device,
        QVulkanDeviceFunctions *_deviceFuncs,
        const SampleCountFlags &_sampleCountFlags,
        const RenderPass &_defaultRenderPass
      ) noexcept;

    public:
      void createCache() noexcept;

    /**
     * Pipeline Worker Helpers/Overloads
     * -------------------------------------------------
     *
     */
    public:
      void createWorkers                (const std::vector<MaterialPtr> &_materials) noexcept;
      void createWorker                 (const std::vector<MaterialPtr> &_materials) noexcept;
      void createWorker                 (const MaterialPtr &_material, bool _isHot = false) noexcept;

    public:
      void waitForWorkersToFinish() noexcept;
      void waitForWorkerToFinish() noexcept;
      bool isWorkerFinished() noexcept;

    /**
     * Destroy Pipeline Helpers
     * -------------------------------------------------
     *
     */
    public:
      void destroyDescriptorPool        (descriptor::Pool &_pool) noexcept;
      void destroyDescriptorSetLayout   (DescLayoutList &_setLayouts) noexcept;
      void destroyDescriptors() noexcept;

      void destroyShaderModules() noexcept;
      void destroyShaderModule          (Shader &_shader) noexcept;

      void destroyTexture               (Texture &_texture) noexcept;
      void destroyTextures() noexcept;

      void destroyPipelines() noexcept;
      void destroyPipeline              (Pipeline &_pipeline) noexcept;
      void destroyPipeline              (const MaterialPtr &_material) noexcept;
      void destroyPipelineLayout        (pipeline::Layout &_pipelineLayout) noexcept;
      void destroyPipelineLayout        (const MaterialPtr &_material) noexcept;
      void destroyRenderPass() noexcept;
      void destroyBuffers() noexcept;

      void swapSDFRPipelines(
        const MaterialPtr &_oldMaterial,
        const MaterialPtr &_newMaterial
      ) noexcept;

    public:
      void setFramebufferAttachments    (const ImageViewList &_fbAttachments) noexcept;
      RenderPass &getRenderPass         (bool _useDefault = true) noexcept;

    /**
     * Create Pipeline Helpers (on Worker Thread)
     * -------------------------------------------------
     *
     * Shader Stages: per Material Pipeline
     * Vertex Layout: per Material Pipeline
     * Pipeline Layout: per Material Pipeline
     */
    private:
      void createPipelines() noexcept;
      void createPipeline               (const MaterialPtr &_material) noexcept;
      void createLayout                 (const MaterialPtr &_material) noexcept;
      void createComputePipeline        (const MaterialPtr &_material) noexcept;
      void createGraphicsPipeline       (const MaterialPtr &_material) noexcept;

      static void initShaderStages      (const MaterialPtr &_material) noexcept;

    /**
     * PSO (Pipeline State Objects) Helpers (on Worker Thread)
     * for Graphics Pipeline ONLY
     * -------------------------------------------------
     *
     */
    private:
      void initPSOs                     (const MaterialPtr &_material) noexcept;

      static void setDynamicState       (const MaterialPtr &_material) noexcept;
      static void setVertexInputState   (const MaterialPtr &_material) noexcept;
      static void setInputAssemblyState (const MaterialPtr &_material) noexcept;
      static void setRasterizationState (const MaterialPtr &_material) noexcept;
      static void setColorBlendState    (const MaterialPtr &_material) noexcept;
      static void setViewportState      (const MaterialPtr &_material) noexcept;
      static void setDepthStencilState  (const MaterialPtr &_material) noexcept;
      void setMultisampleState          (const MaterialPtr &_material) noexcept;

    private:
      Device m_device = VK_NULL_HANDLE;
      QVulkanDeviceFunctions *m_deviceFuncs = VK_NULL_HANDLE;

      pipeline::Cache m_pipelineCache = VK_NULL_HANDLE;
      shader::Module m_currentShaderModule = VK_NULL_HANDLE;

      QMutex m_pipeMutex;

      RenderPassHelper m_renderPassHelper;

      SampleCountFlags m_sampleCountFlags;

      std::vector<MaterialPtr> m_materials;
      std::vector<QFuture<void>> m_workers;
      QFuture<void> m_inclusiveWorker;
      QFuture<void> m_exclusiveWorker;
      bool m_isHot = false; // for swapping old and new pipelines at runtime
  };
}
