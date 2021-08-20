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

//  m_grabBtn->setFocusPolicy(Qt::NoFocus);
//  connect(m_grabBtn, &QPushButton::clicked, this, &MainWindow::onGrabRequested);
//
//  m_quitBtn = new QPushButton(tr("&Quit"));
//  m_quitBtn->setFocusPolicy(Qt::NoFocus);
//  connect(m_quitBtn, &QPushButton::clicked, qApp, &QCoreApplication::quit);
//
//  m_info = new QPlainTextEdit();
//  m_info->setReadOnly(true);
//  m_infoTab = new QTabWidget(this);
//  m_infoTab->addTab(m_info, tr("Vulkan Info"));
//  m_infoTab->addTab(messageLogWidget.data(), tr("Debug Log"));

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

//  m_mainLayout->addWidget(m_infoTab, 2);
  m_mainLayout->addWidget(m_wrapperWidget);
  m_mainLayout->addWidget(m_mainMenuBar);
//  m_mainLayout->addWidget(m_quitBtn, 1);

  m_vpWidget->setLayout(m_mainLayout);
}

void MainWindow::createActions()
{
  m_loadSDFGraphAction = new QAction(tr("Open"), this);
//  m_loadSDFGraphAction->setShortcuts(QKeySequence::Open);
//  m_loadSDFGraphAction->setStatusTip(tr("Open an existing file"));
  connect(
    m_loadSDFGraphAction, &QAction::triggered,
    this, &MainWindow::loadSDFGraph
  );

  m_toggleSDFGraphAction = new QAction(tr("Hide"), this);
  m_toggleSDFGraphAction->setEnabled(false);
//  m_toggleSDFGraphAction->setShortcuts(QKeySequence::Open);
//  m_toggleSDFGraphAction->setStatusTip(tr("Open an existing file"));
  connect(
    m_toggleSDFGraphAction, &QAction::triggered,
    this, &MainWindow::toggleSDFGraph
  );

  m_quitAction = new QAction(tr("Quit"), this);
  m_quitAction->setShortcuts(QKeySequence::Close);
//  openAct->setStatusTip(tr("Open an existing file"));
  connect(
    m_quitAction, &QAction::triggered,
    this, &MainWindow::quitApp
  );


  m_aboutAction = new QAction(tr("About"), this);
//  m_aboutAction->setShortcuts(QKeySequence::Open);
//  m_aboutAction->setStatusTip(tr("Open an existing file"));
  connect(
    m_aboutAction, &QAction::triggered,
    this, &MainWindow::loadAboutDialog
  );


//  auto actionGroup = new QActionGroup(this);
  m_autoCompileAction = new QAction(tr("Auto"), this);
  m_autoCompileAction->setCheckable(true);
  //  m_compileAction->setShortcuts(QKeySequence::Open);
  //  m_compileAction->setStatusTip(tr("Open an existing file"));
  connect(
    m_autoCompileAction, &QAction::toggled,
    this, &MainWindow::autoCompileSDFGraph
  );


  m_compileAction = new QAction(tr("Compile"), this);
//  m_compileAction->setShortcuts(QKeySequence::Open);
//  m_compileAction->setStatusTip(tr("Open an existing file"));
  connect(
    m_compileAction, &QAction::triggered,
    this, &MainWindow::compileSDFGraph
  );


  m_saveAction = new QAction(tr("Save"), this);
//  m_compileAction->setShortcuts(QKeySequence::Open);
//  m_compileAction->setStatusTip(tr("Open an existing file"));
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
  // Signal
  emit m_vkWindow->sdfGraphChanged(m_sdfGraph->getNodes());

  const auto &material = m_vkWindow->getSDFRMaterial();

  m_vkWindow->createSDFRPipeline(material);
}

void MainWindow::autoCompileSDFGraph()
{
  m_compileAction->setEnabled(!m_autoCompileAction->isChecked());
}

void MainWindow::saveSDFNodes()
{

}

void MainWindow::loadSDFGraph()
{
  // creates and stores a fresh new SDFR Material
  const auto &material = m_vkWindow->getSDFRMaterial(true);

  /**
   * @note
   * this will presets the shader's common instructions
   * that are not needed to change
   */
  m_sdfGraph = new SDFGraph(material, this);
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

void MainWindow::loadAboutDialog()
{
//  infoLabel->setText(tr("Invoked <b>Help|About</b>"));
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
//  m_vkWindow->destroyPipeline();

  QApplication::exit(EXIT_SUCCESS);
}
