#pragma once

#include <QFutureWatcher>

#include "Window/VulkanWindow.hpp"
#include "Mesh.hpp"
#include "Camera.hpp"
#include "SDFGraph.hpp"
#include "Pipeline.hpp"

namespace sdfRay4d
{
  using namespace vk;

  class VulkanWindow;

  class Renderer : public QVulkanWindowRenderer, public QObject
  {
    using MaterialPtr = std::shared_ptr<Material>;

    public:
      explicit Renderer(
        VulkanWindow *_vkWindow,
        bool _isMSAA = false
      );

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
     * SDF resource wrappers for targeted external use
     * -------------------------------------------------
     */
    public:
      void createSDFPipeline();
      void destroySDFPipeline();
      MaterialPtr &getSDFMaterial() { return m_sdfrMaterial; }

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

      void initMaterials();
      void initActorMaterial(const PhysicalDeviceLimits *_limits);
      void initSDFRMaterial(const PhysicalDeviceLimits *_limits);

      void initShaders();
      void initActorShaders();
      void initSDFRShaders();

    private:
      void markViewProjDirty();

    /**
     * Resources - Destroy Helpers
     * -------------------------------------------------
     *
     */
    private:
      void destroyBuffers();
      void destroyBuffer(Buffer &_buffer);
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
      void getMatrices(
        QMatrix4x4 *vp,
        QMatrix4x4 *model,
        QMatrix3x3 *modelNormal,
        QVector3D *eyePos
      );
      void writeFragUni(
        quint8 *p,
        const QVector3D &eyePos
      );


      void buildFrame();

      void createBuffers();
      void createBuffer(
        device::Size _size,
        buffer::UsageFlags _usage,
        Buffer &_buffer,
        memory::Reqs &_memReq
      );
      void allocateMemory();
      void bindBufferToMemory(
        const Buffer &_buffer,
        const device::Size &_memOffset
      );
      void mapMemory(size_t _byteSize);
      void updateDescriptorSets();

      void cmdSetViewportAndScissor(
        CmdBuffer &_cmdBuffer,
        uint32_t _extentWidth,
        uint32_t _extentHeight
      ); // TODO: Rename
      void createSDFDrawCalls(
        const MaterialPtr &_material,
        CmdBuffer &_cmdBuffer,
        float _extentWidth,
        float _extentHeight
      );
      void createActorDrawCalls(
        const MaterialPtr &_material,
        CmdBuffer &_cmdBuffer
      );
      void cmdRenderPass();

    private:
      static device::Size aligned(
        device::Size v,
        device::Size byteAlign
      )
      {
        return (v + byteAlign - 1) & ~(byteAlign - 1);
      }

    private:
      Constants m_appConstants;

      bool m_isMSAA;
      bool m_isFramePending = false;

    /**
     * Vulkan Members - Device, Memory & Buffer
     */
    private:
      Device m_device = VK_NULL_HANDLE;

      Buffer m_actorVertexBuffer  = VK_NULL_HANDLE;
      Buffer m_sdfUniformBuffer   = VK_NULL_HANDLE;
      Buffer m_dynamicUniformBuffer = VK_NULL_HANDLE;

      std::vector<Buffer> m_buffers;

      memory::Reqs m_actorVertexMemReq = {};
      memory::Reqs m_sdfUniformMemReq = {};
      memory::Reqs m_dynamicUniformMemReq = {};

      device::Size m_sdfUniformStartOffset = 0;

      device::Memory m_bufferMemory = VK_NULL_HANDLE;

    /**
     * Vulkan Helper Members
     * - Pipeline
     */
    private:
      PipelineHelper m_pipelineHelper;

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
      QFutureWatcher<void> m_frameWatcher;
      QMutex m_guiMutex;

    private:
      float m_verticalAngle = 45.0f;
      float m_nearPlane = 0.01f;
      float m_farPlane = 1000.0f;
      QMatrix4x4 m_proj;
      int m_vpDirty = 0;
      Camera m_camera;
//      QVector3D m_lightPos;

    /**
     * Materials
     */
    private:
      MaterialPtr m_actorMaterial;
      MaterialPtr m_sdfrMaterial;

      Mesh m_actorMesh;

      std::vector<MaterialPtr> m_materials;

      float m_rotation = 0.0f;
  };
}
