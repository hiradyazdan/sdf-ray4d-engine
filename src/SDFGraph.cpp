/*****************************************************
 * Class: SDFGraph (General)
 * Members: General Functions (Public)
 * Partials: None
 * There are however, Data Model Classes with inheritance used in this class
 * which are located in SDFGraph directory
 *****************************************************/

#include <chrono>
#include <thread>

#include "SDFGraph.hpp"

#include "SDFGraph/DataModels/Operations/UnionDataModel.hpp"
#include "SDFGraph/DataModels/Operations/SubtractionDataModel.hpp"

#include "SDFGraph/DataModels/Shapes/CubeDataModel.hpp"
#include "SDFGraph/DataModels/Shapes/SphereDataModel.hpp"
#include "SDFGraph/DataModels/Shapes/TorusDataModel.hpp"

using namespace sdfRay4d;
using namespace sdfGraph;

SDFGraph::SDFGraph(
  VulkanWindow *_vkWindow
) :
  m_vkWindow      (_vkWindow)
, m_shapeMaterial (m_vkWindow->getSDFRMaterial(true)) // creates and stores a fresh new SDFR Material
, m_graphScene    (new FlowScene(registerModels(), this))
, m_graphView     (new FlowView(m_graphScene))
, m_isAutoCompile (false)
{
  setStyle();

  const auto &sdfrShaders = m_appConstants.shadersPaths.raymarch;

  m_shapeMaterial->fragmentShader.preload({
    sdfrShaders.frag.partials.distanceFuncs,
    sdfrShaders.frag.partials.operations,
    sdfrShaders.frag.main
  });
}

void SDFGraph::autoCompile(bool _isAutoCompile)
{
  if(!_isAutoCompile) return;

  // TODO: perhaps to work out queue watcher multi-threaded solution
}

/**
 *
 * @note Qt SLOT
 */
void SDFGraph::compileGraph()
{
  for (const auto &node : getNodes())
  {
    /**
     * @note this seems to be the only way to detect the map node
     * as the nodes order is based on their order of construction
     * at runtime by the user, than static/pre-defined order.
     *
     * as the shader compilation at runtime is naturally expected
     * to have some minor pause/stalling, semi-reflection approach
     * (no reflection in c++) to dynamically check for runtime data,
     * does not cause any major performance cost for the user.
     */
    const auto &mapNode = dynamic_cast<MapDataModel*>(node.second->nodeDataModel());
    if(mapNode)
    {
      qDebug() << "Map NODE data: " << mapNode->getData();
      m_mapNodes.push_back(mapNode);
    }

    const auto &shapeNode = dynamic_cast<ShapeDataModel*>(node.second->nodeDataModel());
    if(shapeNode)
    {
      qDebug() << "shape NODE data: " << shapeNode->getData();
    }

    const auto &opNode = dynamic_cast<OperationDataModel*>(node.second->nodeDataModel());
    if(opNode)
    {
      qDebug() << "operation NODE data: " << opNode->getData();
    }
  }

  std::string shaderData;

  for(const auto &mapNode : m_mapNodes)
  {
    shaderData += "res = ";
    shaderData += mapNode->getData().toStdString();
    shaderData += "\n  ";
  }

  if (m_shapeMaterial->fragmentShader.isValid()) return;

  m_shapeMaterial->fragmentShader.load(shaderData);

  /**
   * @note to avoid any race condition creating
   * a new pipeline needs to be done inside this function
   * after the shader load thread has finished its execution.
   * Because this function is a Qt Slot handling event queues
   * with IPC method on a separate thread.
   */
  m_vkWindow->createSDFRPipeline();
}

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
  registry->registerModel<SubtractionDataModel>(opCatName);

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
