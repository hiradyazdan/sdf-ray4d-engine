/*****************************************************
 * Partial Class: MainWindow
 * Members: Event Handlers - SDF Graph Events (Public)
 *****************************************************/

#include <QtWidgets/QFileDialog>

#include "nodeEditor_old/NodeDataModel.hpp"

#include "Window/MainWindow.hpp"

using namespace sdfRay4d;

void MainWindow::onNodeChanged(const NodePtrSet &_nodes)
{
  if(m_outputNode == nullptr)
  {
    for(const auto &node : _nodes)
    {
      if(node.second->nodeDataModel()->getShaderCode() == "final")
      {
        m_outputNode = node.second.get();
        break;
      }
    }
  }
  else
  {
    std::string shadercode;
    Mat4f translation;
    hsitho::Expressions::flushUnknowns();
    for(const auto &connection : m_outputNode->nodeState().connection(PortType::In, 0))
    {
      if(connection.get() && connection->getNode(PortType::Out).lock())
      {
        shadercode += recurseNodeTree(connection->getNode(PortType::Out).lock(), translation);
      }
    }

    if(!shadercode.empty())
    {
      std::string fragmentShader = m_shaderStart;

      fragmentShader += hsitho::Expressions::getUnknowns();
      fragmentShader += "pos = ";
      fragmentShader += hsitho::Expressions::replaceUnknowns(shadercode);
      fragmentShader += ";";

      fragmentShader += m_shaderEnd;

      m_shaderMan->updateShader(fragmentShader.c_str());
    }
  }
}
