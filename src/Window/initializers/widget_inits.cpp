/*****************************************************
 * Partial Class: MainWindow
 * Members: Constructor init widget & layout helpers (Private)
 *****************************************************/

#include "Window/MainWindow.hpp"

using namespace sdfRay4d;

void MainWindow::initWidgets()
{
  m_wrapperWidget = QWidget::createWindowContainer(m_vkWindow);
  m_wrapperWidget->setFocusPolicy(Qt::StrongFocus);
  m_wrapperWidget->setFocus();

  m_vpWidget = new QWidget();

  setCentralWidget(m_vpWidget);
  setWindowTitle("SDF Ray4D Engine");

  m_mainMenuBar = new QWidget();
  m_mainMenuBar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
}

void MainWindow::initLayouts()
{
  m_mainLayout = new QVBoxLayout();
  m_mainLayout->setContentsMargins(0, 0, 0, 0);

  m_mainLayout->addWidget(m_wrapperWidget);
  m_mainLayout->addWidget(m_mainMenuBar);

  m_vpWidget->setLayout(m_mainLayout);
}

void MainWindow::createActions()
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

  m_quitAction = new QAction(tr("Quit"), this);
  m_quitAction->setShortcuts(QKeySequence::Close);
  connect(
    m_quitAction, &QAction::triggered,
    this, &MainWindow::quitApp
  );


  m_aboutAction = new QAction(tr("About"), this);
//  m_aboutAction->setShortcuts(QKeySequence::Open);
  connect(
    m_aboutAction, &QAction::triggered,
    this, &MainWindow::loadAboutDialog
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

void MainWindow::createMenus()
{
  m_windowMenu = menuBar()->addMenu(tr("Window"));

  m_sdfGraphMenu = m_windowMenu->addMenu(tr("SDF Graph"));
  m_sdfGraphMenu->addAction(m_loadSDFGraphAction);
  m_sdfGraphMenu->addAction(m_toggleSDFGraphAction);

  m_windowMenu->addAction(m_quitAction);

  m_helpMenu = menuBar()->addMenu(tr("Help"));
  m_helpMenu->addAction(m_aboutAction);
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
  innerWidget->setWindowFlags(Qt::Widget);
  m_sdfGraphToolbar = new QToolBar(innerWidget);
  m_sdfGraphToolbar->addAction(m_autoCompileAction);
  m_sdfGraphToolbar->addSeparator();
  m_sdfGraphToolbar->addAction(m_compileAction);
  m_sdfGraphToolbar->addAction(m_saveAction);

  innerWidget->setCentralWidget(sdfGraphView);

  innerWidget->addToolBar(m_sdfGraphToolbar);

  m_sdfGraphWidget->setWidget(innerWidget);
  m_sdfGraphWidget->setVisible(true);

  addDockWidget(Qt::BottomDockWidgetArea, m_sdfGraphWidget);

  m_toggleSDFGraphAction->setEnabled(true);
  m_loadSDFGraphAction->setEnabled(false);

  resizeSDFGraph(dockWidgetArea(m_sdfGraphWidget));

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

  connect(
    m_vkWindow, &VulkanWindow::sdfGraphChanged, // signal/sender (event)
    m_sdfGraph, &SDFGraph::compileGraph // slot/receiver (event handler)
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

void MainWindow::loadAboutDialog()
{
  QMessageBox::about(
    this,
    tr("About Menu"),
    tr(
      "The <b>Menu</b> example shows how to create "
      "menu-bar menus and context menus."
    )
  );
}

void MainWindow::quitApp()
{
  QApplication::exit(EXIT_SUCCESS);
}
