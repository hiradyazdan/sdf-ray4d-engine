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

#include <SDFGraph/SDFGraphDataModel.hpp>

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
    using TypeConverterId = QtNodes::TypeConverterId;

    using NodePtrSet = std::unordered_map<QUuid, std::unique_ptr<QtNodes::Node>>;
    using NodePtrList = std::vector<Node*>;

    public:
      explicit SDFGraph(VulkanWindow *_vkWindow);

    public:
      FlowView *getView() { return m_graphView; }
      const NodePtrSet &getNodes() { return m_graphScene->nodes(); }

    public slots:
      void compileGraph(const SDFGraph::NodePtrSet &_nodes);

    private:
      static std::shared_ptr<DataModelRegistry> registerModels();
      static void setStyle();

    private:
      Constants m_appConstants;

      VulkanWindow *m_vkWindow;
      FlowScene *m_graphScene;
      FlowView *m_graphView;

    private:
      std::shared_ptr<Material> m_shapeMaterial;
  };
}
