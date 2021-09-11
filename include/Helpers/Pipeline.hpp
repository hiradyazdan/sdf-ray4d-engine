#pragma once

#include <QtConcurrentRun>
#include <QSize>

#include "BaseHelper.hpp"

#include "Descriptor.hpp"
#include "Buffer.hpp"
#include "Command.hpp"
#include "Framebuffer.hpp"
#include "RenderPass.hpp"

namespace sdfRay4d::helpers
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

    public:
      BufferHelper buffer;
      DescriptorHelper descriptor;
      CommandHelper command;

    public:
      void initHelpers(
        const Device &_device,
        QVulkanDeviceFunctions *_deviceFuncs,
        const SampleCountFlags &_sampleCountFlags,
        const RenderPass &_defaultRenderPass
      );
      void initSwapChainHelpers         (const texture::ImageView &_fbAttachments);

    public:
      void createCache();

    /**
     * Pipeline Worker Helpers/Overloads
     * -------------------------------------------------
     *
     */
    public:
      void createWorkers                (const std::vector<MaterialPtr> &_materials);
      void createWorker                 (const std::vector<MaterialPtr> &_materials);
      void createWorker                 (const MaterialPtr &_material, bool _isHot = false);

    public:
      void waitForWorkersToFinish();
      void waitForWorkerToFinish();
      bool isWorkerFinished();

    /**
     * Destroy Pipeline Helpers
     * -------------------------------------------------
     *
     */
    public:
      void destroyDescriptorPool        (descriptor::Pool &_pool);
      void destroyDescriptorSetLayout   (DescLayoutList &_setLayouts);
      void destroyDescriptors();

      void destroyShaderModules();
      void destroyShaderModule          (Shader &_shader);

      void destroyTexture               (Texture &_texture);
      void destroyTextures();

      void destroyPipelines();
      void destroyPipeline              (Pipeline &_pipeline);
      void destroyPipeline              (const MaterialPtr &_material);
      void destroyPipelineLayout        (pipeline::Layout &_pipelineLayout);
      void destroyPipelineLayout        (const MaterialPtr &_material);
      void destroyRenderPass();
      void destroyBuffers();

      void swapSDFRPipelines(
        const MaterialPtr &_oldMaterial,
        const MaterialPtr &_newMaterial
      );

    public:
      RenderPass &getRenderPass         (bool _useDefault = true);

    /**
     * Create Pipeline Helpers (on Worker Thread)
     * -------------------------------------------------
     *
     * Shader Stages: per Material Pipeline
     * Vertex Layout: per Material Pipeline
     * Pipeline Layout: per Material Pipeline
     */
    private:
      void createPipelines();
      void createPipeline               (const MaterialPtr &_material);
      void createLayout                 (const MaterialPtr &_material);
      void createComputePipeline        (const MaterialPtr &_material);
      void createGraphicsPipeline       (const MaterialPtr &_material);

      static void initShaderStages      (const MaterialPtr &_material);

    /**
     * PSO (Pipeline State Objects) Helpers (on Worker Thread)
     * for Graphics Pipeline ONLY
     * -------------------------------------------------
     *
     */
    private:
      void initPSOs                     (const MaterialPtr &_material);

      static void setDynamicState       (const MaterialPtr &_material);
      static void setVertexInputState   (const MaterialPtr &_material);
      static void setInputAssemblyState (const MaterialPtr &_material);
      static void setRasterizationState (const MaterialPtr &_material);
      static void setColorBlendState    (const MaterialPtr &_material);
      static void setViewportState      (const MaterialPtr &_material);
      static void setDepthStencilState  (const MaterialPtr &_material);
      void setMultisampleState          (const MaterialPtr &_material);

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
