#include "Window/MainWindow.hpp"

#include "nodeEditor_old/DataModelRegistry.hpp"
#include "nodeEditor_old/NodeGraphicsObject.hpp"

#include "SDFGraph/DistanceFieldOutputDataModel.hpp"
#include "SDFGraph/Primitives/CubePrimitiveDataModel.hpp"

using namespace sdfRay4d;

void MainWindow::initSDFGraph()
{
  registerModels();

  m_sdfGraphScene = new FlowScene(this);
  m_sdfGraphView = new FlowView(m_sdfGraphScene);

  setupOutputNode();
}

void MainWindow::registerModels()
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

void MainWindow::setupOutputNode()
{
  // Create the static distance field output node
  auto node = m_sdfGraphScene->createNode(
    std::make_unique<DistanceFieldOutputDataModel>(),
    false,
    QUuid("ffffffff-ffff-ffff-ffff-ffffffffffff")
  );
  QRectF geom = m_sdfGraphView->sceneRect();

  QPoint pos;
  QRectF box = node->nodeGraphicsObject()->boundingRect();
  qreal x, y, w, h;
  geom.getRect(&x, &y, &w, &h);
  std::cout << w << " " << h << "\n";
  pos.setX(w/2 + box.width());
  pos.setY(h/2 + box.height());
  QPointF posView = m_sdfGraphView->mapToScene(pos);

  QFont f;
  f.setPixelSize(36);
  node->nodeGeometry().setSpacing(5);
  node->nodeGeometry().recalculateSize(QFontMetrics(f));
  node->nodeGraphicsObject()->setPos(posView);

  m_sdfGraphView->centerOn(node->nodeGraphicsObject().get());
}
