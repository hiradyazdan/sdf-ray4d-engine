#pragma once

//#include <shaderc/shaderc.hpp>

#include <QFutureWatcher>
#include <QtConcurrentRun>

//#include "Window/MainWindow.hpp"
#include "Window/VulkanWindow.hpp"

#include "Shader.hpp"

namespace sdfRay4d
{
  using namespace vk;

  class VulkanWindow;

  class Renderer : public QVulkanWindowRenderer
  {
    public:
      explicit Renderer(VulkanWindow *_vkWindow, bool _isMSAA = false);

      /**
       * Resource Initializers
       */
      void preInitResources() override;
      void initResources() override;
      void initSwapChainResources() override;

      /**
       * Resource Destructors
       */
      void releaseResources() override;
      void releaseSwapChainResources() override;

      /**
       * Draw/Paint Frame
       */
      void startNextFrame() override;

    private:
      pipeline::VertexInputInfo createVertexInputState();
      pipeline::ShaderStageInfo *createShaderStages();

      void loadShaders();

      /**
       * Create Pipeline Helpers
       *
       * Pipeline Cache: Once
       *
       * Shader Stages: per Pipeline/Material
       * Vertex Layout: per Pipeline/Material
       * Descriptor Sets/Layouts: per Pipeline/Material
       * Pipeline Layout: per Pipeline/Material
       */
      void createPipelines();

      void createPipelineCache();

      void createObjPipeline();

      void setShaderStages();
      void setVertexLayout();
      void setupDescriptorSets();
      void createPipelineLayout();
      void createGraphicsPipeline();

      /**
       * Create Buffer
       */
//      void createUploadBuffer(int _concurrentFrameCount);
//
//      void createBuffer();
//      void allocateMemory();
//      void mapBufferMemory();

      void buildFrame();
      void ensureBuffers();
      void ensureInstanceBuffer();
      void buildDrawCallsForItems();
      void buildDrawCallsForFloor();

      void markViewProjDirty();

      /**
       * Destroy Resources
       */
      void destroyPipeline();
      void destroyDescriptorSets();
      void destroyBuffers();
      void destroyShaderModules();

    private:
      pipeline::InputAssemblyInfo setInputAssemblyInfo();
      pipeline::RasterizationInfo setRasterizationInfo();
      pipeline::ViewportInfo setViewportInfo();
      pipeline::MultisampleInfo setMultisampleInfo();
      pipeline::DepthStencilInfo setDepthStencilInfo();
      pipeline::ColorBlendInfo setColorBlendInfo();

    private:
      static device::Size aligned(device::Size v, device::Size byteAlign)
      {
        return (v + byteAlign - 1) & ~(byteAlign - 1);
      }

    private:
      struct {
        device::Size        vertUniSize;
        device::Size        fragUniSize;
        device::Size        uniMemStartOffset;

        Shader              vertexShader;
        Shader              fragmentShader;

        descriptor::Pool    descPool = VK_NULL_HANDLE;
        descriptor::Layout  descSetLayout = VK_NULL_HANDLE;
        descriptor::Set     descSet;

        pipeline::Layout    pipelineLayout = VK_NULL_HANDLE;
        Pipeline            pipeline = VK_NULL_HANDLE;
      } m_objMaterial;

    private:
      bool m_isMSAA;
      bool m_isFramePending;

      /**
       * Qt Vulkan Members
       */
      VulkanWindow *m_vkWindow;
      QVulkanInstance *m_vkInstance;
      QVulkanDeviceFunctions *m_deviceFuncs;

      /**
       * Qt Members - Multi-threading
       */
      QFuture<void> m_pipelinesFuture;
      QFutureWatcher<void> m_frameWatcher;
      QMutex m_guiMutex;

      QMatrix4x4 m_proj;
      int m_vpDirty = 0;

      /**
       * Vulkan Members - Device, Memory & Buffer
       */
      Device m_device = VK_NULL_HANDLE;
//      vma::Allocator m_allocator;
      device::Memory m_bufMem = VK_NULL_HANDLE;
      Buffer m_buf = VK_NULL_HANDLE;
      Buffer m_uniformBuffer = VK_NULL_HANDLE;

      /**
       * Vulkan Members - Descriptor Sets
       */
      descriptor::BufferInfo m_uniformBufInfo[QVulkanWindow::MAX_CONCURRENT_FRAME_COUNT];
      descriptor::Set m_descSet[QVulkanWindow::MAX_CONCURRENT_FRAME_COUNT];

      /**
       * Vulkan Members - Pipeline
       */
      pipeline::Cache m_pipelineCache = VK_NULL_HANDLE;
      pipeline::ShaderStageInfo m_shaderStages[2];
      pipeline::VertexInputInfo m_vertexInputInfo;

      float m_rotation = 0.0f;

      static const int UNIFORM_DATA_SIZE = 16 * sizeof(float);

      // Note that the vertex data and the projection matrix assume OpenGL. With
      // Vulkan Y is negated in clip space and the near/far plane is at 0/1 instead
      // of -1/1. These will be corrected for by an extra transformation when
      // calculating the modelview-projection matrix.
      constexpr static const float vertexData[] = { // Y up, front = CCW
        0.0f,   0.5f,   1.0f, 0.0f, 0.0f,
        -0.5f,  -0.5f,   0.0f, 1.0f, 0.0f,
        0.5f,  -0.5f,   0.0f, 0.0f, 1.0f
      };
  };
}
