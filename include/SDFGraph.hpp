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
      void setAutoCompile(bool _isAutoCompile = false);
      void terminateAutoCompile() { m_isAutoCompile = false; }

    public slots:
      void compile(bool _isAutoCompile = false);
      void removeMapNode(const sdfGraph::Connection &_connection);

    private:
      static DataModelRegistryPtr registerModels();
      static void setStyle();

    private:
      void autoCompile();
      const NodePtrMap &getNodes() { return m_graphScene->nodes(); }
      void setMapNodes();
      void setMapNodeConnections(const sdfGraph::MapDataModel *_mapDataModel) const;

    private:
      /**
       * @tparam[in] TDataModel
       * @param[in] _node
       * @return pointer to TDataModel instance
       */
      template<typename TDataModel>
      TDataModel *getDataModel(
        const sdfGraph::Node *_node
      ) const noexcept
      { return dynamic_cast<TDataModel*>(_node->nodeDataModel()); }

    private:
      VulkanWindow *m_vkWindow = VK_NULL_HANDLE;
      FlowScene *m_graphScene = nullptr;
      FlowView *m_graphView = nullptr;

    private:
      MaterialPtr m_sdfrMaterial = VK_NULL_HANDLE;
      MapDataModelPtrSet m_mapNodes;

      bool m_isAutoCompile = false;
      bool m_isMapNodeRemoved = false;
      QFuture<void> m_worker;
  };
}
