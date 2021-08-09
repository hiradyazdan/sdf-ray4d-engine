#pragma once

#include <QFutureWatcher>
#include <QtConcurrentRun>

#include "Window/VulkanWindow.hpp"
#include "Camera.hpp"
#include "SDFGraph.hpp"
#include "Material.hpp"

namespace sdfRay4d
{
  using namespace vk;

  class VulkanWindow;

  class Renderer : public QVulkanWindowRenderer, public QObject
  {
    using MaterialPtr = std::shared_ptr<Material>;

    public:
      explicit Renderer(VulkanWindow *_vkWindow, bool _isMSAA = false);

    public slots:
      void updateFrame();

    /**
     * Resource Initializers/Destructors
     * -------------------------------------------------
     */
    public:
      void preInitResources() override;
      void initResources() override;
      void releaseResources() override;

    /**
     * SwapChain Resource Initializers/Destructors
     * -------------------------------------------------
     */
    public:
      void initSwapChainResources() override;
      void releaseSwapChainResources() override;

      /**
       * Draw/Paint Frame
       */
      void startNextFrame() override;

    public:
      void createSDFPipeline();
      void destroySDFPipeline();
      MaterialPtr &getSDFMaterial() { return m_sdfMaterial; }

    /**
     * Frame - User Input Helpers/Handlers
     * -------------------------------------------------
     */
    public:
      void yaw(float degrees);
      void pitch(float degrees);
      void walk(float amount);
      void strafe(float amount);

    /**
     * Resources: Init Helpers (General)
     * -------------------------------------------------
     *
     */
    private:
      void initVkFunctions();
      void initSDFShaders();

    /**
     * Resources: Create Pipeline Helpers (on Worker Thread)
     * -------------------------------------------------
     *
     * Pipeline Cache: Once
     *
     * Shader Stages: per Material Pipeline
     * Vertex Layout: per Material Pipeline
     * Descriptor Sets/Layouts: per Material Pipeline
     * Pipeline Layout: per Material Pipeline
     */
    private:
      void createPipelineCache();

      void createPipelineWorker(
        QFuture<void> &_pipeline,
        const MaterialPtr &_material
      );

      void createMaterialPipeline(const MaterialPtr &_material);
      void createPipelineLayout(const MaterialPtr &_material);
      void createComputePipeline(const MaterialPtr &_material);
      void createGraphicsPipeline(const MaterialPtr &_material);

      static void initShaderStages(const MaterialPtr &_material);
      void setupDescriptorSets(const MaterialPtr &_material);

    /**
     * Resources: Init PSO (Pipeline State Objects) Helpers (on Worker Thread)
     * -------------------------------------------------
     *
     */
    private:
      void initPSOs(const MaterialPtr &_material);

      static void setDynamicState(const MaterialPtr &_material);
      static void setVertexInputState(const MaterialPtr &_material);
      static void setInputAssemblyState(const MaterialPtr &_material);
      static void setRasterizationState(const MaterialPtr &_material);
      static void setColorBlendState(const MaterialPtr &_material);
      static void setViewportState(const MaterialPtr &_material);
      static void setDepthStencilState(const MaterialPtr &_material);
      void setMultisampleState(const MaterialPtr &_material);

      void markViewProjDirty();

    /**
     * Resources - Destroy Helpers
     * -------------------------------------------------
     *
     */
    private:
      void destroyPipelines();
      void destroyPipeline(const MaterialPtr &_material);
      void destroyPipelineLayout(const MaterialPtr &_material);
      void destroyDescriptorSets();
      void destroyBuffers();
      void destroyShaderModules();
      void destroyShaderModule(Shader &_shader);

    /**
     * Frame Helpers (on Worker Thread)
     * -------------------------------------------------
     * - Buffers
     * - Draw Calls
     * - Render Pass
     */
    private:
      void buildFrame();
      void createBuffers();
      void buildDrawCalls(
        const MaterialPtr &_material,
        CmdBuffer &_cmdBuffer,
        QSize &_swapChainImgSize
      );
      void cmdRenderPass();

    private:
      static device::Size aligned(device::Size v, device::Size byteAlign)
      {
        return (v + byteAlign - 1) & ~(byteAlign - 1);
      }

    private:
      bool m_isMSAA;
      bool m_isFramePending = false;

    /**
     * Vulkan Members - Device, Memory & Buffer
     */
    private:
      Device m_device = VK_NULL_HANDLE;
      device::Memory m_bufMem = VK_NULL_HANDLE;
      Buffer m_buf = VK_NULL_HANDLE;
      Buffer m_uniformBuffer = VK_NULL_HANDLE;

    /**
     * Vulkan Members - Descriptor Sets
     */
    private:
//      descriptor::BufferInfo m_uniformBufInfo[QVulkanWindow::MAX_CONCURRENT_FRAME_COUNT];
//      descriptor::Set m_descSet[QVulkanWindow::MAX_CONCURRENT_FRAME_COUNT];

    /**
     * Vulkan Members - Pipeline
     */
    private:
      pipeline::Cache m_pipelineCache = VK_NULL_HANDLE;

    /**
     * Qt Vulkan Members
     */
    private:
      VulkanWindow *m_vkWindow;
      QVulkanDeviceFunctions *m_deviceFuncs = VK_NULL_HANDLE;

    /**
     * Qt Members - Multi-threading
     */
    private:
      QFuture<void> m_sdfPipelineWorker;
      QFutureWatcher<void> m_frameWatcher;
      QMutex m_guiMutex;

    private:
      QMatrix4x4 m_proj;
      int m_vpDirty = 0;
      Camera m_camera;

    /**
     * Materials
     */
    private:
      // Use this material for mesh (non-sdf-raymarched) objects
      MaterialPtr m_objMaterial;
      MaterialPtr m_sdfMaterial;

      std::vector<MaterialPtr> m_materials;

      float m_rotation = 0.0f;
  };
}
