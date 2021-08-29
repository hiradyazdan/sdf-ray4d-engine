#pragma once

#include <QDebug>

#include <nodes/FlowScene>
#include <nodes/FlowView>
#include <nodes/Node>
#include <nodes/DataModelRegistry>
#include <nodes/ConnectionStyle>

#include "SDFGraph/DataModels/MapDataModel.hpp"

#include "Window/VulkanWindow.hpp"
#include "Material.hpp"

namespace sdfRay4d
{
  class VulkanWindow;

  class SDFGraph : public QWidget
  {
    using FlowScene         = QtNodes::FlowScene;
    using FlowView          = QtNodes::FlowView;

    using DataModelRegistryPtr  = std::shared_ptr<QtNodes::DataModelRegistry>;
    using NodePtr               = std::unique_ptr<QtNodes::Node>;
    using NodePtrSet            = std::unordered_map<QUuid, NodePtr>;

    using MaterialPtr       = std::shared_ptr<Material>;
    using MapDataModelList  = std::vector<sdfGraph::MapDataModel*>;

    public:
      explicit SDFGraph(VulkanWindow *_vkWindow);

    public:
      FlowView *getView() { return m_graphView; }
      void autoCompile(bool _isAutoCompile = false);

    public slots:
      void compileGraph();

    private:
      static DataModelRegistryPtr registerModels();
      static void setStyle();

    private:
      const NodePtrSet &getNodes() { return m_graphScene->nodes(); }

    private:
      Constants m_appConstants;

      VulkanWindow *m_vkWindow;
      FlowScene *m_graphScene;
      FlowView *m_graphView;

    private:
      MaterialPtr m_shapeMaterial;
      MapDataModelList m_mapNodes;

      bool m_isAutoCompile;
  };
}
