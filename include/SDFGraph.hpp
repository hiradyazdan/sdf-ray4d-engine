#pragma once

#include <QDebug>

#include <nodes/FlowScene>
#include <nodes/FlowView>
#include <nodes/Node>
#include <nodes/DataModelRegistry>
#include <nodes/ConnectionStyle>

#include "Window/VulkanWindow.hpp"

#include "SDFGraph/DataModels/MapDataModel.hpp"

namespace sdfRay4d
{
  class VulkanWindow;

  class SDFGraph : public QWidget
  {
    using FlowScene             = QtNodes::FlowScene;
    using FlowView              = QtNodes::FlowView;

    using DataModelRegistryPtr  = std::shared_ptr<QtNodes::DataModelRegistry>;
    using NodePtr               = std::unique_ptr<QtNodes::Node>;
    using NodePtrMap            = std::unordered_map<QUuid, NodePtr>;

    using MaterialPtr           = std::shared_ptr<Material<float>>;
    using MapDataModelPtrSet    = std::unordered_set<sdfGraph::MapDataModel*>;

    public:
      explicit SDFGraph(VulkanWindow *_vkWindow);

    public:
      FlowView *getView() { return m_graphView; }
      void setAutoCompileConnection(bool _isAutoCompile = false);

    public slots:
      void compile();
      void removeMapNode(const sdfGraph::Connection &_connection);

    private:
      static DataModelRegistryPtr registerModels();
      static void setStyle();

    private:
      const NodePtrMap &getNodes() { return m_graphScene->nodes(); }
      void findMapNodes();

    private:
      template<typename TDataModel>
      TDataModel *getDataModel(const sdfGraph::Node *_node)
      const noexcept { return dynamic_cast<TDataModel*>(_node->nodeDataModel()); }

    private:
      Constants m_appConstants;

      VulkanWindow *m_vkWindow = VK_NULL_HANDLE;
      FlowScene *m_graphScene = nullptr;
      FlowView *m_graphView = nullptr;

    private:
      MaterialPtr m_sdfrMaterial = VK_NULL_HANDLE;
      MapDataModelPtrSet m_mapNodes;

      bool m_isAutoCompile = false;
      bool m_isMapNodeRemoved = false;
  };
}
