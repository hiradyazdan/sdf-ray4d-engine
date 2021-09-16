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

  namespace sdfrShaders = Constants::shadersPaths::raymarch::frag;

  m_sdfrMaterial->fragmentShader.preload({
    sdfrShaders::partials::distanceFuncs,
    sdfrShaders::partials::operations,
    sdfrShaders::main
  });

  connect(
    m_graphScene, &FlowScene::connectionDeleted,
    this, &SDFGraph::removeMapNode
  );
}

/**
 * @note Qt SLOT
 * 
 * @param[in] _connection
 */
void SDFGraph::removeMapNode(const Connection &_connection)
{
  const auto &node = _connection.getNode(PortType::In);
  const auto &mapNode = getDataModel<MapDataModel>(node);

  if(mapNode)
  {
    m_mapNodes.erase(mapNode);
    m_isMapNodeRemoved = true;

    if(m_isAutoCompile)
    {
      autoCompile();
    }
  }
}

void SDFGraph::setMapNodes()
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
    if(mapNode && !m_isMapNodeRemoved)
    {
      setMapNodeConnections(mapNode);

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
 * @param[in] _mapDataModel
 */
void SDFGraph::setMapNodeConnections(const MapDataModel *_mapDataModel) const
{
  if(m_isAutoCompile)
  {
    connect(
      _mapDataModel, &MapDataModel::isValid,
      this, &SDFGraph::autoCompile,
      Qt::UniqueConnection
    );
  }
  else
  {
    disconnect(
      _mapDataModel, &MapDataModel::isValid,
      this, &SDFGraph::autoCompile
    );
  }
}

/**
 *
 * @param[in] _isAutoCompile
 */
void SDFGraph::setAutoCompile(bool _isAutoCompile)
{
  m_isAutoCompile = _isAutoCompile;

  /**
   * @note if set auto is requested after creation of valid
   * nodes and connections, compile will not be invoked as
   * signal-slot are not connected at that point. Also, if
   * in manual compile the nodes were invalidated (removed)
   * and then switch to auto, they need to be compiled as
   * the map node isValid signal does not emit at that point
   *
   * So, need to manually compile on selecting auto, if a
   * map node exists and has shader data (is valid).
   */
  compile();

  m_worker = QtConcurrent::run([this]()
  {
    /**
     * @note we only need to find the map nodes
     * and set the map node connections through an infinite
     * loop in a background process, which creates dynamic/runtime
     * connections for signal/slot between dynamic map data and the
     * compile function. This will allow the auto compile to get
     * triggered only when the map node is ready and valid.
     */
    while(true)
    {
      if(!m_isAutoCompile) break;

      setMapNodes();

      /**
       * @note this one millisecond makes the whole difference
       * when removing a connection or node and/or switch to manual
       * compile after, otherwise the app will crash.
       *
       * TODO: Figure out why and replace with a better logic
       */
      std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
  });
}

/**
 * @note Qt SLOT
 *
 * @param[in] _isAutoCompile
 */
void SDFGraph::compile(bool _isAutoCompile)
{
  if(!_isAutoCompile)
  {
    /**
     * @note this will prevent race condition to happen intermittently
     * when switching between auto compile and manual compile
     */
    m_worker.waitForFinished();

    setMapNodes();
  }

  std::string shaderData;

  for(const auto &mapNode : m_mapNodes)
  {
    const auto &mapData = mapNode->getData().toStdString();

    if(mapData.empty()) continue;

    shaderData += "res = ";
    shaderData += mapData;
    shaderData += "\n  ";
  }

  if(
    m_sdfrMaterial->fragmentShader.isValid() ||
    (shaderData.empty() && !m_isMapNodeRemoved)
  ) return;

  m_sdfrMaterial->fragmentShader.load(shaderData);

  /**
   * @note to avoid any race condition creating
   * a new pipeline needs to be done inside this function
   * after the shader load thread has finished its execution.
   * Because this function is a Qt Slot handling event queues
   * with IPC method on a separate thread.
   */
  m_vkWindow->createSDFRPipeline();

  /**
   * @note this has to be set back to false after shader
   * compilation and when other relevant workers finished
   * their job, otherwise the infinite loop of setMapNodes
   * in the separate thread (for auto compile) will repopulate
   * the already deleted map node which can cause race condition
   * issue and crashes the application.
   */
  m_isMapNodeRemoved = false;
}

void SDFGraph::autoCompile()
{
  compile(true);
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
