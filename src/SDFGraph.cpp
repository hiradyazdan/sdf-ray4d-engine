#include <fstream>
#include <utility>

#include "nodeEditor_old/DataModelRegistry.hpp"
#include "nodeEditor_old/NodeGraphicsObject.hpp"

#include "SDFGraph.hpp"

#include "SDFGraph/Primitives/CubePrimitiveDataModel.hpp"
#include "SDFGraph/DistanceFieldOutputDataModel.hpp"
#include "SDFGraph/CollapsedNodeDataModel.hpp"

using namespace sdfRay4d;

SDFGraph::SDFGraph(
  std::shared_ptr<Material> _material,
  QWidget *_scene
) :
  m_shapeMaterial(std::move(_material))
  , m_outputNode(nullptr)
{
//  if (!m_shapeMaterial.vertexShader.isValid())
//  {
//    m_shapeMaterial.vertexShader.load(
//      "static/fullscreentri.vert.spv"
//    );
//  }
//
//  if (!m_shapeMaterial.fragmentShader.isValid())
//  {
//    m_shapeMaterial.fragmentShader.load(
//      "dynamic/rtprimitives.frag",
//      {
//        "_partials/distance_functions.partial.glsl",
//        "_partials/operations.partial.glsl"
//      }
//    );
//  }

  registerModels();

  m_graphScene = new FlowScene(_scene/*this*/);
  m_graphView = new FlowView(m_graphScene);

  createMapNode();

  const auto &raymarchShaders = m_appConstants.shadersPaths.raymarch;

  m_shapeMaterial->fragmentShader.preload(
//    "",
    {
      raymarchShaders.frag.partials.distanceFuncs,
      raymarchShaders.frag.partials.operations,
//      "",
      raymarchShaders.frag.main
//      "dynamic/shader.begin.frag",
//      "",
//      "dynamic/shader.end.frag"
    }
  );

//  std::ifstream s("shaders/shader.begin.frag");
//  std::ifstream e("shaders/shader.end.frag");
//  m_shaderStart = std::string((std::istreambuf_iterator<char>(s)), std::istreambuf_iterator<char>());
//  m_shaderEnd = std::string((std::istreambuf_iterator<char>(e)), std::istreambuf_iterator<char>());
}

void SDFGraph::registerModels()
{
  // Register the nodes
  DataModelRegistry::registerModel<CubePrimitiveDataModel>("Shapes");
//  DataModelRegistry::registerModel<TorusPrimitiveDataModel>("Primitives");
//  DataModelRegistry::registerModel<SpherePrimitiveDataModel>("Shapes");
//  DataModelRegistry::registerModel<CylinderPrimitiveDataModel>("Primitives");
//  DataModelRegistry::registerModel<CapsulePrimitiveDataModel>("Primitives");
//  DataModelRegistry::registerModel<PlanePrimitiveDataModel>("Primitives");
//  DataModelRegistry::registerModel<ConePrimitiveDataModel>("Primitives");
//  DataModelRegistry::registerModel<TriangularPrismPrimitiveDataModel>("Primitives");
//  DataModelRegistry::registerModel<HexagonalPrismPrimitiveDataModel>("Primitives");

//	DataModelRegistry::registerModel<UnionDataModel>("Transformations");
//  DataModelRegistry::registerModel<SubtractionOpDataModel>("Transformations");
//  DataModelRegistry::registerModel<IntersectionDataModel>("Operations");
//  DataModelRegistry::registerModel<BlendDataModel>("Operations");
//
//	DataModelRegistry::registerModel<TranslateDataModel>("Transforms");
//	DataModelRegistry::registerModel<ScaleDataModel>("Transforms");
//  DataModelRegistry::registerModel<RotateDataModel>("Transforms");
//
//	DataModelRegistry::registerModel<VectorDataModel>("Maths");
//  DataModelRegistry::registerModel<ScalarDataModel>("Maths");
//  DataModelRegistry::registerModel<SineDataModel>("Maths");
//  DataModelRegistry::registerModel<CosineDataModel>("Maths");
//  DataModelRegistry::registerModel<MultiplyDataModel>("Maths");
//  DataModelRegistry::registerModel<DivideDataModel>("Maths");
//	DataModelRegistry::registerModel<AdditionDataModel>("Maths");
//	DataModelRegistry::registerModel<SubtractionDataModel>("Maths");

//	DataModelRegistry::registerModel<TimeDataModel>("Misc");
//	DataModelRegistry::registerModel<ColorPickerDataModel>("Color");
//  DataModelRegistry::registerModel<OutputDataModel>("Generic");
//	DataModelRegistry::registerModel<InputDataModel>("Generic");
//	DataModelRegistry::registerModel<CopyDataModel>("Generic");
//	DataModelRegistry::registerModel<CopyNumDataModel>("Generic");
//	DataModelRegistry::registerModel<CollapsedNodeDataModel>("Generic");
}

void SDFGraph::createMapNode()
{
  // Create the static distance field output node
  auto node = m_graphScene->createNode(
    std::make_unique<DistanceFieldOutputDataModel>(),
    false,
    QUuid("ffffffff-ffff-ffff-ffff-ffffffffffff")
  );
  QRectF geom = m_graphView->sceneRect();

  QPoint pos;
  QRectF box = node->nodeGraphicsObject()->boundingRect();
  qreal x, y, w, h;
  geom.getRect(&x, &y, &w, &h);
  std::cout << w << " " << h << "\n";
  pos.setX(w/2 + box.width());
  pos.setY(h/2 + box.height());
  QPointF posView = m_graphView->mapToScene(pos);

  QFont f;
  f.setPixelSize(36);
  node->nodeGeometry().setSpacing(5);
  node->nodeGeometry().recalculateSize(QFontMetrics(f));
  node->nodeGraphicsObject()->setPos(posView);

  m_graphView->centerOn(node->nodeGraphicsObject().get());
}

void SDFGraph::compileGraph(const NodePtrSet &_nodes)
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

  if(m_outputNode != nullptr)
  {
    std::string shaderData;
    Mat4f translation;
    hsitho::Expressions::flushUnknowns();
    for(const auto &connection : m_outputNode->nodeState().connection(PortType::In, 0))
    {
      if(connection.get() && connection->getNode(PortType::Out).lock())
      {
        shaderData += recurseNodeTree(connection->getNode(PortType::Out).lock(), translation);
      }
    }

    if(!shaderData.empty())
    {
      std::string fragmentShader;// = m_shaderStart;

//      fragmentShader += hsitho::Expressions::getUnknowns();
//      fragmentShader += "vec2 map(in vec3 pos)";
//      fragmentShader += "vec4 map(vec3 _position)";
//      fragmentShader += "\n{";
        fragmentShader += "res = opU( res, vec2( sdBox(pos-vec3( 1.0,0.25, 0.0), vec3(0.25) ), 3.0 ) );";
//      fragmentShader += "\n\tvec4 pos = ";
//      fragmentShader += hsitho::Expressions::replaceUnknowns(shaderData);
//      fragmentShader += ";";
//      fragmentShader += "\n\treturn res;";
//      fragmentShader += "\n\treturn pos;";
//      fragmentShader += "\n}";

      //fragmentShader += m_shaderEnd;

//      m_shapeMaterial->vertexShader = ;
if (!m_shapeMaterial->fragmentShader.isValid())
{
  m_shapeMaterial->fragmentShader.load(
    fragmentShader
  );
}
//
//      m_shaderMan->updateShader(fragmentShader.c_str());
    }
  }
}

std::string SDFGraph::recurseNodeTree(
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
