/*****************************************************
 * Partial Class: MainWindow
 * Members: Constructor init widget & layout helpers (Private)
 *****************************************************/

#include <QtWidgets/QPushButton>

#include "Window/MainWindow.hpp"

using namespace sdfRay4d;

void MainWindow::initWidgets()
{
  m_wrapperWidget = QWidget::createWindowContainer(m_vkWindow);
  m_wrapperWidget->setFocusPolicy(Qt::StrongFocus);
  m_wrapperWidget->setFocus();

//  m_grabBtn = new QPushButton(tr("&Grab"));
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

  m_dockWidget = new QDockWidget(tr("SDF Graph Editor"), this);
  m_dockWidget->setAllowedAreas(
    Qt::TopDockWidgetArea |
    Qt::BottomDockWidgetArea |
    Qt::LeftDockWidgetArea |
    Qt::RightDockWidgetArea
  );
  m_dockWidget->setWidget(m_sdfGraphView);

  setCentralWidget(m_vpWidget);
  addDockWidget(Qt::BottomDockWidgetArea, m_dockWidget);
}

void MainWindow::initLayouts()
{
  m_mainLayout = new QVBoxLayout();
  m_mainLayout->setContentsMargins(0, 0, 0, 0);

//  m_mainLayout->addWidget(m_infoTab, 2);
  m_mainLayout->addWidget(m_wrapperWidget);
//  m_mainLayout->addWidget(m_grabBtn, 1);
//  m_mainLayout->addWidget(m_quitBtn, 1);

  m_vpWidget->setLayout(m_mainLayout);
}
