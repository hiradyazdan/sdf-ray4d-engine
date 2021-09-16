#pragma once

#include <QFutureWatcher>

#include "Helpers/Pipeline.hpp"
#include "Window/VulkanWindow.hpp"
#include "Mesh.hpp"
#include "Camera.hpp"
#include "SDFGraph.hpp"

namespace sdfRay4d
{
  using namespace vk;

  class VulkanWindow;

  class Renderer : public QVulkanWindowRenderer, public QObject
  {
    using Material    = Material<float>;
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
      MaterialPtr &getSDFRMaterial(bool _isNew = false);
      void createSDFRPipeline();
      void swapSDFRPipelines();

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

    /**
     * Resources: Init Materials Helpers
     * -------------------------------------------------
     *
     */
    private:
      void initMaterials();
      void initDepthMaterial();
      void initActorMaterial();
      void initSDFRMaterial();
      void initSDFRMaterial(const MaterialPtr &_material);

    /**
     * Resources: Init Shaders Helpers
     * -------------------------------------------------
     *
     */
    private:
      void initShaders();
      void initDepthShaders();
      void initActorShaders();
      void initSDFRShaders();

    /**
     * Swapchain Resource Helpers
     * -------------------------------------------------
     *
     */
    private:
      void createDepthView();

    private:
      void markViewProjDirty();

    /**
     * Frame Helpers (on Worker Thread)
     * -------------------------------------------------
     * - Buffers
     * - Update Descriptor Sets
     * - Commands
     */
    private:
      void buildFrame();
      void createBuffers();
      void updateDescriptorSets();
      void executeCommands();

    private:
      const VkPhysicalDeviceLimits *getDeviceLimits() const;
      device::Size setDynamicOffsetAlignment(
        device::Size _offset
      );

    private:
      bool m_isMSAA;
      bool m_isFramePending = false;

    /**
     * Vulkan Members
     */
    private:
      Device m_device = VK_NULL_HANDLE;

    /**
     * Vulkan Helper Members
     * - Pipeline
     */
    private:
      helpers::PipelineHelper m_pipelineHelper;

    /**
     * Qt Vulkan Members
     */
    private:
      VulkanWindow *m_vkWindow = VK_NULL_HANDLE;
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
      int m_concurrentFrameCount = 0;
      Camera m_camera;
      QSize m_windowSize;
//      QVector3D m_lightPos;

    /**
     * Materials
     */
    private:
      MaterialPtr m_depthMaterial   = VK_NULL_HANDLE;
      MaterialPtr m_actorMaterial   = VK_NULL_HANDLE;
      MaterialPtr m_sdfrMaterial    = VK_NULL_HANDLE;
      MaterialPtr m_newSDFRMaterial = VK_NULL_HANDLE;

      Mesh m_actorMesh;

      std::vector<MaterialPtr> m_materials;

      float m_rotation = 0.0f;

      bool m_isNewWorker = false;
  };
}
