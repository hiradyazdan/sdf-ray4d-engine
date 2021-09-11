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

  createSDFGraphActions();
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

void MainWindow::loadAboutDialog()
{
  QMessageBox::about(
    this,
    tr("About Menu"),
    tr(
      "4D SDF Raymarching Engine..."
    )
  );
}

void MainWindow::quitApp()
{
  if(m_sdfGraph)
  {
    m_sdfGraph->setAutoCompileConnection(false);
  }

  QApplication::exit(EXIT_SUCCESS);
}
