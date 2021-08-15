#pragma once

#include <QtConcurrentRun>
#include <QSize>

#include "Material.hpp"
#include "RenderPass.hpp"

namespace sdfRay4d
{
  using namespace vk;

  /**
   * @class PipelineHelper
   * @brief
   *
   */
  class PipelineHelper
  {
//    using ImageViews  = std::vector<std::reference_wrapper<texture::ImageView>>;
    using MaterialPtr = std::shared_ptr<Material>;

    public:
      void init(
        const Device &_device,
        QVulkanDeviceFunctions *_deviceFuncs,
        const SampleCountFlags &_sampleCountFlags,
        const RenderPass &_defaultRenderPass,
        texture::ImageView *_attachments
      );

    public:
      void createCache();
      void createWorkers(const std::vector<MaterialPtr> &_materials);
      void createWorker(const MaterialPtr &_material);

    public:
      void waitForWorkersToFinish();

    public:
      void destroyDescriptors();
      void destroyPipelines();
      void destroyPipeline              (const MaterialPtr &_material);
      void destroyPipelineLayout        (const MaterialPtr &_material);

    public:
      RenderPass getRenderPass(bool _useDefault = true);
      Framebuffer getFramebuffer(uint32_t _width, uint32_t _height);

    /**
     * Create Pipeline Helpers (on Worker Thread)
     * -------------------------------------------------
     *
     * Shader Stages: per Material Pipeline
     * Vertex Layout: per Material Pipeline
     * Descriptor Sets/Layouts: per Material Pipeline
     * Pipeline Layout: per Material Pipeline
     */
    private:
      void createPipelines();
      void createPipeline               (const MaterialPtr &_material);
      void createLayout                 (const MaterialPtr &_material);
      void createComputePipeline        (const MaterialPtr &_material);
      void createGraphicsPipeline       (const MaterialPtr &_material);

      static void initShaderStages      (const MaterialPtr &_material);

      void createDescriptorSets         (const MaterialPtr &_material);
      void createDescriptorPool         (const MaterialPtr &_material);
      void createDescriptorSetLayout    (const MaterialPtr &_material);
      void allocateDescriptorSets       (const MaterialPtr &_material);

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
      void createFramebuffer(uint32_t _width, uint32_t _height);

    private:
      Device m_device = VK_NULL_HANDLE;
      QVulkanDeviceFunctions *m_deviceFuncs = VK_NULL_HANDLE;

      RenderPassHelper m_renderPassHelper;
      SampleCountFlags m_sampleCountFlags;

      RenderPass m_renderPass = VK_NULL_HANDLE;

      Framebuffer m_frameBuffer = VK_NULL_HANDLE;
      pipeline::Cache m_pipelineCache = VK_NULL_HANDLE;

      std::vector<MaterialPtr> m_materials;
      std::vector<QFuture<void>> m_workers;

      texture::ImageView *m_attachments;
  };
}
