#pragma once

#include <QFutureWatcher>
#include <QtConcurrentRun>

#include "Window/VulkanWindow.hpp"

#include "PSO.hpp"
#include "Shader.hpp"

namespace sdfRay4d
{
  using namespace vk;

  class VulkanWindow;

  class Renderer : public QVulkanWindowRenderer, public QObject
  {
    using ShaderStageInfoList = std::vector<pipeline::ShaderStageInfo>;

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

    /**
     * Resources: Init Helpers (General)
     * -------------------------------------------------
     *
     */
    private:
      void initVkFunctions();
      void initShaders();

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
      void createPipelines();
      void createPipelineCache();
      void createObjPipeline();
      void createPipelineLayout();
      void createGraphicsPipeline();

      void initShaderStages();
      void setupDescriptorSets();

    /**
     * Resources: Init PSO (Pipeline State Objects) Helpers (on Worker Thread)
     * -------------------------------------------------
     *
     */
    private:
      void initPSOs();

      void setDynamicState();
      void setVertexInputState();
      void setInputAssemblyState();
      void setRasterizationState();
      void setColorBlendState();
      void setViewportState();
      void setDepthStencilState();
      void setMultisampleState();

      void markViewProjDirty();

    /**
     * Resources - Destroy Helpers
     * -------------------------------------------------
     *
     */
    private:
      void destroyPipelines();
      void destroyDescriptorSets();
      void destroyBuffers();
      void destroyShaderModules();

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
      struct
      {
        device::Size        vertUniSize;
        device::Size        fragUniSize;
        device::Size        uniMemStartOffset;

        Shader              vertexShader;
        Shader              fragmentShader;

        descriptor::Pool    descPool = VK_NULL_HANDLE;
        descriptor::Layout  descSetLayout = VK_NULL_HANDLE;
        descriptor::Set     descSet;

        pipeline::Layout    pipelineLayout = VK_NULL_HANDLE;
        PSO                 pso;
        ShaderStageInfoList shaderStages;

        Pipeline            pipeline = VK_NULL_HANDLE;
      } m_objMaterial;

    private:
      bool m_isMSAA;
      bool m_isFramePending;

    /**
     * Qt Vulkan Members
     */
    private:
      VulkanWindow *m_vkWindow;
      QVulkanInstance *m_vkInstance;
      QVulkanDeviceFunctions *m_deviceFuncs;

    /**
     * Qt Members - Multi-threading
     */
    private:
      QFuture<void> m_pipelinesFuture;
      QFutureWatcher<void> m_frameWatcher;
      QMutex m_guiMutex;

    private:
      QMatrix4x4 m_proj;
      int m_vpDirty = 0;

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
      descriptor::BufferInfo m_uniformBufInfo[QVulkanWindow::MAX_CONCURRENT_FRAME_COUNT];
      descriptor::Set m_descSet[QVulkanWindow::MAX_CONCURRENT_FRAME_COUNT];

    /**
     * Vulkan Members - Pipeline
     */
    private:
      pipeline::Cache m_pipelineCache = VK_NULL_HANDLE;

      float m_rotation = 0.0f;
  };
}
