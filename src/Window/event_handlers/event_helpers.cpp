/*****************************************************
 * Partial Class: MainWindow
 * Members: Event Helpers (Private)
 *****************************************************/

#include <QtWidgets/QFileDialog>

#include "SDFGraph/CollapsedNodeDataModel.hpp"

#include "Window/MainWindow.hpp"

using namespace sdfRay4d;

std::string MainWindow::recurseNodeTree(
  const std::shared_ptr<Node> &_node,
  Mat4f _t, PortIndex portIndex,
  unsigned int _cp
)
{
  unsigned int iter = 1;
  std::string shadercode;
  _t.setCpn(_cp);
  _node->nodeDataModel()->setCopyNum(_cp);

  if(_node->nodeDataModel()->getNodeType() == DFNodeType::TRANSFORM)
  {
    _t = _t * _node->nodeDataModel()->getTransform();
  }
  else if(_node->nodeDataModel()->getNodeType() == DFNodeType::PRIMITIVE)
  {
    _node->nodeDataModel()->setTransform(_t);
    shadercode += _node->nodeDataModel()->getShaderCode();
  }
  else if(_node->nodeDataModel()->getNodeType() == DFNodeType::MIX)
  {
    shadercode += _node->nodeDataModel()->getShaderCode();
  }
  else if(_node->nodeDataModel()->getNodeType() == DFNodeType::COPY)
  {
    iter = boost::lexical_cast<unsigned int>(_node->nodeDataModel()->getShaderCode());
  }

  for(unsigned int it = 0; it < iter; ++it)
  {
    if(iter > 1)
    {
      _cp = it;
      if(iter - it > 1)
      {
        shadercode += "opUnion(";
      }
    }
    std::vector<std::shared_ptr<Connection>> inConns = _node->nodeState().connection(PortType::In);
    if(_node->nodeDataModel()->getNodeType() == DFNodeType::COLLAPSED) {
      std::vector<std::shared_ptr<Connection>> inConnsTmp;
      std::shared_ptr<Node> o = dynamic_cast<CollapsedNodeDataModel *>(_node->nodeDataModel().get())->getOutputs()[portIndex];
      for(auto &c : o->nodeState().connection(PortType::In)) {
        inConnsTmp.push_back(c);
      }
      inConns.swap(inConnsTmp);
      inConnsTmp.clear();
    }

    unsigned int i = 0;
    for(auto connection : inConns)
    {
      if(connection.get() && connection->getNode(PortType::Out).lock()) {
        ++i;
        shadercode += recurseNodeTree(connection->getNode(PortType::Out).lock(), _t, connection->getPortIndex(PortType::Out), _cp);
        if(_node->nodeDataModel()->getNodeType() == DFNodeType::MIX) {
          if(i < inConns.size())
            shadercode += ",";
          else
            shadercode += _node->nodeDataModel()->getExtraParams() + ")";
        }
      }
    }
    if(iter > 1 && iter - it > 1)
    {
      shadercode += ",";
    }
  }
  if(iter > 1)
  {
    for(unsigned int it = 0; it < iter - 1; ++it)
      shadercode += ")";
  }
  return shadercode;
}

void MainWindow::onVulkanInfoReceived(const QString &text)
{
  m_info->setPlainText(text);
}

void MainWindow::onFrameQueued(int colorValue)
{
//  m_number->display(colorValue);
}

void MainWindow::onGrabRequested()
{
  if (!m_vkWindow->supportsGrab()) {
    QMessageBox::warning(this, tr("Cannot grab"), tr("This swapchain does not support readbacks."));
    return;
  }

  QImage img = m_vkWindow->grab();

  // Our startNextFrame() implementation is synchronous so img is ready to be
  // used right here.

  QFileDialog fd(this);
  fd.setAcceptMode(QFileDialog::AcceptSave);
  fd.setDefaultSuffix("png");
  fd.selectFile("test.png");
  if (fd.exec() == QDialog::Accepted)
    img.save(fd.selectedFiles().first());
}
