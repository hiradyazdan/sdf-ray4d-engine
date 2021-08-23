#pragma once

#include <QDebug>

#include <nodes/FlowScene>
#include <nodes/FlowView>
#include <nodes/Node>
#include <nodes/NodeData>
#include <nodes/ConnectionStyle>
#include <nodes/Connection>
#include <nodes/NodeGeometry>

#include <nodes/DataModelRegistry>
#include <nodes/internal/NodeGraphicsObject.hpp>

#include "SDFGraph/DataModels/MapDataModel.hpp"

#include "Window/VulkanWindow.hpp"
#include "Material.hpp"

namespace sdfRay4d
{
  class VulkanWindow;

  class SDFGraph : public QWidget
  {
    using FlowScene = QtNodes::FlowScene;
    using FlowView = QtNodes::FlowView;
    using Connection = QtNodes::Connection;
    using ConnectionStyle = QtNodes::ConnectionStyle;
    using Node = QtNodes::Node;
    using PortIndex = QtNodes::PortIndex;
    using PortType = QtNodes::PortType;

    using DataModelRegistry = QtNodes::DataModelRegistry;
    using TypeConverter = QtNodes::TypeConverter;

    using NodePtrSet = std::unordered_map<QUuid, std::unique_ptr<QtNodes::Node>>;

    public:
      explicit SDFGraph(VulkanWindow *_vkWindow);

    public:
      FlowView *getView() { return m_graphView; }
      void autoCompile(bool _isAutoCompile = false);

    public slots:
      void compileGraph();

    private:
      static std::shared_ptr<DataModelRegistry> registerModels();
      static void setStyle();

    private:
      const NodePtrSet &getNodes() { return m_graphScene->nodes(); }

    private:
      Constants m_appConstants;

      VulkanWindow *m_vkWindow;
      FlowScene *m_graphScene;
      FlowView *m_graphView;

    private:
      std::shared_ptr<Material> m_shapeMaterial;
      bool m_isAutoCompile;

      std::vector<sdfGraph::MapDataModel*> m_mapNodes;
  };
}
