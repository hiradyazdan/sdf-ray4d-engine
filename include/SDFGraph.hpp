#pragma once

#include "nodeEditor_old/FlowScene.hpp"
#include "nodeEditor_old/FlowView.hpp"
#include "nodeEditor_old/Node.hpp"

#include "Window/VulkanWindow.hpp"
#include "Material.hpp"

namespace sdfRay4d
{
  class VulkanWindow;

  class SDFGraph : public QWidget
  {
    using NodePtrSet = std::unordered_map<QUuid, std::shared_ptr<Node>>;

    public:
      SDFGraph(
        std::shared_ptr<Material> &_material,
        QWidget *_scene
      );

    public:
      FlowView *getView() { return m_graphView; }
      NodePtrSet getNodes() { return m_graphScene->getNodes(); }

    public slots:
      void compileGraph(const sdfRay4d::SDFGraph::NodePtrSet &_nodes);

    private:
      void registerModels();
      void createMapNode();

      std::string recurseNodeTree(
        const std::shared_ptr<Node> &_node,
        Mat4f _t,
        PortIndex portIndex = 0,
        unsigned int _cp = 0
      );

    /**
     * Qt/Vulkan Members
     */
    private:
//      VulkanWindow *m_vkWindow;
//      Device m_device = VK_NULL_HANDLE;
//      QVulkanDeviceFunctions *m_deviceFuncs = VK_NULL_HANDLE;

    private:
      std::shared_ptr<Material> m_shapeMaterial;
//      std::shared_ptr<Material> m_shapeMaterial;

    private:
      FlowScene *m_graphScene;
      FlowView *m_graphView;

      Node *m_outputNode;

      Constants m_appConstants;
  };
}
