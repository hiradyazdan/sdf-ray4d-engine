/*****************************************************
 * Partial Class: MainWindow
 * Members: SDF Graph init helpers (Private)
 *****************************************************/

#include "Window/MainWindow.hpp"

using namespace sdfRay4d;

void MainWindow::createSDFGraphActions()
{
  m_loadSDFGraphAction = new QAction(tr("Open"), this);
  connect(
    m_loadSDFGraphAction, &QAction::triggered,
    this, &MainWindow::loadSDFGraph
  );

  m_toggleSDFGraphAction = new QAction(tr("Hide"), this);
  m_toggleSDFGraphAction->setEnabled(false);
//  m_toggleSDFGraphAction->setShortcuts(QKeySequence::Open);
  connect(
    m_toggleSDFGraphAction, &QAction::triggered,
    this, &MainWindow::toggleSDFGraph
  );

  m_autoCompileAction = new QAction(tr("Auto"), this);
  m_autoCompileAction->setCheckable(true);
  //  m_compileAction->setShortcuts(QKeySequence::Open);
  connect(
    m_autoCompileAction, &QAction::toggled,
    this, &MainWindow::autoCompileSDFGraph
  );

  m_compileAction = new QAction(tr("Compile"), this);
//  m_compileAction->setShortcuts(QKeySequence::Open);
  connect(
    m_compileAction, &QAction::triggered,
    this, &MainWindow::compileSDFGraph
  );

  m_saveAction = new QAction(tr("Save"), this);
//  m_compileAction->setShortcuts(QKeySequence::Open);
  connect(
    m_saveAction, &QAction::triggered,
    this, &MainWindow::saveSDFNodes
  );
}

void MainWindow::compileSDFGraph()
{
  // Signal for compile SDF Graph (Slot = SDFGraph::compileGraph)
  emit m_vkWindow->sdfGraphChanged();
}

void MainWindow::autoCompileSDFGraph()
{
  const auto &isAutoCompile = m_autoCompileAction->isChecked();

  m_compileAction->setEnabled(!isAutoCompile);
  m_sdfGraph->autoCompile(isAutoCompile);
}

void MainWindow::saveSDFNodes()
{

}

void MainWindow::loadSDFGraph()
{
  /**
   * @note
   * this will presets the shader's common instructions
   * that are not needed to change
   */
  m_sdfGraph = new SDFGraph(m_vkWindow);
  m_sdfGraphWidget = new QDockWidget(tr("SDF Graph Editor"), this);
  m_sdfGraphWidget->setAllowedAreas(
    Qt::TopDockWidgetArea
    | Qt::BottomDockWidgetArea
    | Qt::LeftDockWidgetArea
    | Qt::RightDockWidgetArea
  );

  auto &&sdfGraphView = m_sdfGraph->getView();
  auto innerWidget = new QMainWindow(m_sdfGraphWidget);

  m_sdfGraphToolbar = new QToolBar(innerWidget);
  m_sdfGraphToolbar->addAction(m_autoCompileAction);
  m_sdfGraphToolbar->addSeparator();
  m_sdfGraphToolbar->addAction(m_compileAction);
  m_sdfGraphToolbar->addAction(m_saveAction);

  innerWidget->setWindowFlags(Qt::Widget);
  innerWidget->addToolBar(m_sdfGraphToolbar);
  innerWidget->setCentralWidget(sdfGraphView);

  m_sdfGraphWidget->setWidget(innerWidget);
  m_sdfGraphWidget->setVisible(true);

  addDockWidget(Qt::BottomDockWidgetArea, m_sdfGraphWidget);

  m_toggleSDFGraphAction->setEnabled(true);
  m_loadSDFGraphAction->setEnabled(false);

  resizeSDFGraph(dockWidgetArea(m_sdfGraphWidget));

  createSDFGraphWidgetConnections();

  connect(
    m_vkWindow, &VulkanWindow::sdfGraphChanged, // signal/sender (event)
    m_sdfGraph, &SDFGraph::compileGraph // slot/receiver (event handler)
  );
}

/**
 * @brief creates widget window related connections
 * @note
 * - resize SDF Graph \n
 * - toggle SDF Graph Menu \n
 * - toggle SDF Graph Top Level (allowing undocked widget window to maximize/restore down)
 */
void MainWindow::createSDFGraphWidgetConnections()
{
  connect(
    m_sdfGraphWidget, &QDockWidget::dockLocationChanged, // signal/sender (event)
    this, &MainWindow::resizeSDFGraph // slot/receiver (event handler)
  );
  connect(
    m_sdfGraphWidget, &QDockWidget::visibilityChanged, // signal/sender (event)
    this, &MainWindow::toggleSDFGraphMenu // slot/receiver (event handler)
  );
  connect(
    m_sdfGraphWidget, &QDockWidget::topLevelChanged, // signal/sender (event)
    this, &MainWindow::toggleSDFGraphTopLevel // slot/receiver (event handler)
  );
}

void MainWindow::toggleSDFGraph()
{
  auto isSDFGraphVisible = m_sdfGraphWidget->isVisible();

  m_sdfGraphWidget->setVisible(!isSDFGraphVisible);
}

void MainWindow::toggleSDFGraphMenu(bool _isVisible)
{
  m_toggleSDFGraphAction->setText(_isVisible ? "Hide" : "Show");
}

void MainWindow::toggleSDFGraphTopLevel()
{
  if (m_sdfGraphWidget->isFloating())
  {
    m_sdfGraphWidget->setWindowFlags(
      Qt::CustomizeWindowHint
      | Qt::Window
      | Qt::WindowMinimizeButtonHint
      | Qt::WindowMaximizeButtonHint
      | Qt::WindowCloseButtonHint
    );
    m_sdfGraphWidget->show();
  }
}
