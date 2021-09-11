/*****************************************************
 * Class: SDFGraph (General)
 * Members: General Functions (Public/Private)
 * Partials: None
 * There are however, Data Model Classes with inheritance used in this class
 * which are located in SDFGraph directory:
 * - Data Models
 *      - Operations
 *          - UnionDataModel
 *      - Shapes
 *          - CubeDataModel
 *          - SphereDataModel
 *          - TorusDataModel
 * - MapDataModel
 * - OperationDataModel
 * - ShapeDataModel
 *****************************************************/

#include "SDFGraph.hpp"

#include "SDFGraph/DataModels/Operations/UnionDataModel.hpp"
#include "SDFGraph/DataModels/Operations/SubtractionDataModel.hpp"

#include "SDFGraph/DataModels/Shapes/CubeDataModel.hpp"
#include "SDFGraph/DataModels/Shapes/SphereDataModel.hpp"
#include "SDFGraph/DataModels/Shapes/TorusDataModel.hpp"

using namespace sdfRay4d;
using namespace sdfGraph;

/**
 *
 * @param[in] _vkWindow
 */
SDFGraph::SDFGraph(
  VulkanWindow *_vkWindow
) :
  m_vkWindow    (_vkWindow)
, m_sdfrMaterial(_vkWindow->getSDFRMaterial(true)) // creates and stores a fresh new SDFR Material
, m_graphScene  (new FlowScene(registerModels(), this))
, m_graphView   (new FlowView(m_graphScene))
{
  setStyle();

  const auto &sdfrShaders = m_appConstants.shadersPaths.raymarch;

  m_sdfrMaterial->fragmentShader.preload({
    sdfrShaders.frag.partials.distanceFuncs,
    sdfrShaders.frag.partials.operations,
    sdfrShaders.frag.main
  });

  connect(
    m_graphScene, &FlowScene::connectionDeleted,
    this, &SDFGraph::removeMapNode
  );
}

void SDFGraph::removeMapNode(const Connection &_connection)
{
  const auto &node = _connection.getNode(PortType::In);
  const auto &mapNode = getDataModel<MapDataModel>(node);

  if(mapNode)
  {
    m_mapNodes.erase(mapNode);
    m_isMapNodeRemoved = true;
  }

  if(m_isAutoCompile)
  {
    compile();
  }
}

void SDFGraph::findMapNodes()
{
  for(const auto &node : getNodes())
  {
    const auto &nodeValue = node.second;

    if(!nodeValue) continue;

    /**
     * @note this seems to be the only way to detect the map node
     * as the nodes order is based on their order of construction
     * at runtime by the user, than static/pre-defined order.
     *
     * @note also, flowScene's Signal-Slot connections don't work as
     * they trigger right before the node data is transferred through
     * the connections.
     *
     * @note as the shader compilation at runtime is naturally expected
     * to have some minor pause/stalling, semi-reflection approach
     * (no reflection in c++) to dynamically check for runtime data,
     * does not cause any major performance cost for the user.
     */
    const auto &mapNode = getDataModel<MapDataModel>(nodeValue.get());
    if(mapNode)
    {
      if(m_isAutoCompile)
      {
        connect(
          mapNode, &MapDataModel::isValid,
          this, &SDFGraph::compile,
          Qt::UniqueConnection
        );
      }
      else
      {
        disconnect(
          mapNode, &MapDataModel::isValid,
          this, &SDFGraph::compile
        );
      }

      qDebug() << "Map NODE data: " << mapNode->getData();
      m_mapNodes.insert(mapNode);
    }

    if(!m_isAutoCompile) // TODO: Use debug compile def for this
    {
      const auto &shapeNode = getDataModel<ShapeDataModel>(nodeValue.get());
      if(shapeNode)
      {
        qDebug() << "shape NODE data: " << shapeNode->getData();
      }

      const auto &opNode = getDataModel<OperationDataModel>(nodeValue.get());
      if(opNode)
      {
        qDebug() << "operation NODE data: " << opNode->getData();
      }
    }
  }
}

/**
 *
 * @param[in] _isAutoCompile
 */
void SDFGraph::setAutoCompileConnection(bool _isAutoCompile)
{
  m_isAutoCompile = _isAutoCompile;

  QtConcurrent::run([this]()
  {
    while(true)
    {
      if(!m_isAutoCompile)
      {
        findMapNodes();
        break;
      }

      findMapNodes();
    }
  });
}

/**
 *
 * @note Qt SLOT
 */
void SDFGraph::compile()
{
  findMapNodes();

  std::string shaderData;

  for(const auto &mapNode : m_mapNodes)
  {
    const auto &mapNodeData = mapNode->getData().toStdString();

    if(mapNodeData.empty()) continue;

    shaderData += "res = ";
    shaderData += mapNodeData;
    shaderData += "\n  ";
  }

  if(
    m_sdfrMaterial->fragmentShader.isValid() ||
    shaderData.empty() && !m_isMapNodeRemoved
  ) return;

  m_isMapNodeRemoved = false;

  m_sdfrMaterial->fragmentShader.load(shaderData);

  /**
   * @note to avoid any race condition creating
   * a new pipeline needs to be done inside this function
   * after the shader load thread has finished its execution.
   * Because this function is a Qt Slot handling event queues
   * with IPC method on a separate thread.
   */
  m_vkWindow->createSDFRPipeline();
}

/**
 *
 * @return DataModelRegistryPtr instance
 */
SDFGraph::DataModelRegistryPtr SDFGraph::registerModels()
{
  const auto &registry      = std::make_shared<DataModelRegistry>();

  const auto &shapeCatName  = "a) Shapes";
  const auto &opCatName     = "b) Operations";
  const auto &mapCatName    = "c) Final Outputs";

  registry->registerModel<CubeDataModel>(shapeCatName);
  registry->registerModel<SphereDataModel>(shapeCatName);
  registry->registerModel<TorusDataModel>(shapeCatName);

  registry->registerModel<UnionDataModel>(opCatName);
//  registry->registerModel<SubtractionDataModel>(opCatName); // FIXME

  registry->registerModel<MapDataModel>(mapCatName);

  return registry;
}

void SDFGraph::setStyle()
{
  ConnectionStyle::setConnectionStyle(
    R"(
    {
      "ConnectionStyle": {
        "ConstructionColor": "gray",
        "NormalColor": "black",
        "SelectedColor": "gray",
        "SelectedHaloColor": "deepskyblue",
        "HoveredColor": "deepskyblue",

        "LineWidth": 3.0,
        "ConstructionLineWidth": 2.0,
        "PointDiameter": 10.0,

        "UseDataDefinedColors": true
      }
    }
  )");
}
