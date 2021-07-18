#pragma once

//#include <shaderc/shaderc.hpp>

#include "Window/MainWindow.hpp"

namespace sdfRay4d
{
  using namespace vk;

  class Renderer : public QVulkanWindowRenderer
  {
    public:
      explicit Renderer(QVulkanWindow *_vkWindow, bool _msaa = false);

      void initResources() override;
      void releaseResources() override;

      void initSwapChainResources() override;
      void releaseSwapChainResources() override;

      void startNextFrame() override;
//      void updateShader(std::string &_shaderCode);

    private:
      ShaderModule createShader(const QString &_filePath, ShaderStageFlags _stage);

      pipeline::VertexInputState createVertexInputState();
      pipeline::ShaderStage *createShaderStages();

      /**
       * Create Buffer & Descriptor Sets
       */
      void createUploadBuffer(int _concurrentFrameCount);
      void setupDescriptorSets(int _concurrentFrameCount);

      /**
       * Create Pipeline
       */
      void createPipelineCache();
      void createPipelineLayout();
      void createGraphicsPipeline();

      /**
       * Destroy Resources
       */
      void destroyPipeline();
      void destroyDescriptorSets();
      void destroyBuffers();

    private:
      static device::Size aligned(device::Size v, device::Size byteAlign)
      {
        return (v + byteAlign - 1) & ~(byteAlign - 1);
      }

    private:
      /**
       * Qt Vulkan Members
       */
      QVulkanWindow *m_vkWindow;
      QVulkanDeviceFunctions *m_deviceFuncs;

      /**
       * Vulkan Members - Device & Buffer
       */
      Device m_device = VK_NULL_HANDLE;
      device::Memory m_bufMem = VK_NULL_HANDLE;
      Buffer m_buf = VK_NULL_HANDLE;

      /**
       * Vulkan Members - Descriptor Sets
       */
      descriptor::BufferInfo m_uniformBufInfo[QVulkanWindow::MAX_CONCURRENT_FRAME_COUNT];
      descriptor::Set m_descSet[QVulkanWindow::MAX_CONCURRENT_FRAME_COUNT];
      descriptor::Layout m_descSetLayout = VK_NULL_HANDLE;
      descriptor::Pool m_descPool = VK_NULL_HANDLE;

      /**
       * Vulkan Members - Pipeline
       */
      pipeline::Cache m_pipelineCache = VK_NULL_HANDLE;
      pipeline::Layout m_pipelineLayout = VK_NULL_HANDLE;
      Pipeline m_pipeline = VK_NULL_HANDLE;

      QMatrix4x4 m_proj;
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
