#include <QtWidgets/QPushButton>
#include <QtWidgets/QFileDialog>

#include <QLoggingCategory>
#include <QPointer>
#include <QLibraryInfo>

#include "MainWindow.hpp"
#include "Renderer.hpp"

using namespace ray4d;

Q_LOGGING_CATEGORY(lcVk, "qt.vulkan")

static QPointer<QPlainTextEdit> messageLogWidget;
static QtMessageHandler oldMessageHandler = nullptr;

static void messageHandler(QtMsgType _msgType, const QMessageLogContext &_logContext, const QString &_text)
{
  if (!messageLogWidget.isNull()) messageLogWidget->appendPlainText(_text);
  if (oldMessageHandler) oldMessageHandler(_msgType, _logContext, _text);
}

void MainWindow::initVkInstance()
{
  m_vkInstance = new QVulkanInstance();

  /**
   * TODO:
   * Validation layers in Qt seem to be skipped
   * when using Debug mode on CLion
   */
  const QByteArrayList &validationLayers = {
    "VK_LAYER_KHRONOS_validation"
  };

  for(const auto &layer : validationLayers)
  {
    if (!m_vkInstance->supportedLayers().contains(layer))
    {
      qFatal("This Validation layer isn't supported: " + layer);
    }
  }

  // Enable validation layer,
  // if supported. Messages go to qDebug by default.
  m_vkInstance->setLayers(validationLayers);

  if (!m_vkInstance->create())
  {
    qFatal("Failed to create Vulkan instance: %d", m_vkInstance->errorCode());
  }
}

void MainWindow::initVkWindow()
{
  m_vkWindow = new VulkanWindow();
  m_vkWindow->setVulkanInstance(m_vkInstance);
}

void MainWindow::initWidgets()
{
  m_wrapperWidget = QWidget::createWindowContainer(m_vkWindow);

  m_info = new QPlainTextEdit();
  m_info->setReadOnly(true);

  m_grabBtn = new QPushButton(tr("&Grab"));
  m_grabBtn->setFocusPolicy(Qt::NoFocus);

  connect(m_grabBtn, &QPushButton::clicked, this, &MainWindow::onGrabRequested);

  m_quitBtn = new QPushButton(tr("&Quit"));
  m_quitBtn->setFocusPolicy(Qt::NoFocus);

  connect(m_quitBtn, &QPushButton::clicked, qApp, &QCoreApplication::quit);

  m_infoTab = new QTabWidget(this);
  m_infoTab->addTab(m_info, tr("Vulkan Info"));
  m_infoTab->addTab(messageLogWidget.data(), tr("Debug Log"));

  m_vpWidget = new QWidget();

  setCentralWidget(m_vpWidget);
}

void MainWindow::initLayouts()
{
  m_mainLayout = new QVBoxLayout();

  m_mainLayout->addWidget(m_infoTab, 2);
  m_mainLayout->addWidget(m_wrapperWidget, 5);
  m_mainLayout->addWidget(m_grabBtn, 1);
  m_mainLayout->addWidget(m_quitBtn, 1);
}

MainWindow::MainWindow()
{
  messageLogWidget = new QPlainTextEdit(QLatin1String(QLibraryInfo::build()) + QLatin1Char('\n'));
  messageLogWidget->setReadOnly(true);

  oldMessageHandler = qInstallMessageHandler(messageHandler);

  QLoggingCategory::setFilterRules(QStringLiteral("qt.vulkan=true"));

  initVkInstance();
  initVkWindow();
  initWidgets();
  initLayouts();

  m_vpWidget->setLayout(m_mainLayout);

  connect(m_vkWindow, &VulkanWindow::vulkanInfoReceived, this, &MainWindow::onVulkanInfoReceived);
//  connect(m_vkWindow, &VulkanWindow::frameQueued, this, &MainWindow::onFrameQueued);
}

void MainWindow::onVulkanInfoReceived(const QString &text)
{
  m_info->setPlainText(text);
}

void MainWindow::onFrameQueued(int colorValue)
{
//  m_number->display(colorValue);
}

void MainWindow::onGrabRequested()
{
  if (!m_vkWindow->supportsGrab()) {
    QMessageBox::warning(this, tr("Cannot grab"), tr("This swapchain does not support readbacks."));
    return;
  }

  QImage img = m_vkWindow->grab();

  // Our startNextFrame() implementation is synchronous so img is ready to be
  // used right here.

  QFileDialog fd(this);
  fd.setAcceptMode(QFileDialog::AcceptSave);
  fd.setDefaultSuffix("png");
  fd.selectFile("test.png");
  if (fd.exec() == QDialog::Accepted)
    img.save(fd.selectedFiles().first());
}

QPalette MainWindow::setPalette()
{
  QPalette palette;

  /**
   * https://gist.github.com/skyrpex/5547015
   */

  palette.setColor(QPalette::Window, QColor(53,53,53));
  palette.setColor(QPalette::WindowText, Qt::white);

  palette.setColor(QPalette::Base, QColor(25,25,25));
  palette.setColor(QPalette::AlternateBase, QColor(53,53,53));
  palette.setColor(QPalette::ToolTipBase, Qt::white);

  palette.setColor(QPalette::ToolTipText, Qt::white);

  palette.setColor(QPalette::Text, Qt::white);
  palette.setColor(QPalette::Button, QColor(53,53,53));
  palette.setColor(QPalette::ButtonText, Qt::white);
  palette.setColor(QPalette::BrightText, Qt::red);

  palette.setColor(QPalette::Link, QColor(42, 130, 218));

  palette.setColor(QPalette::Highlight, QColor(42, 130, 218));
  palette.setColor(QPalette::HighlightedText, Qt::black);

  palette.setColor(QPalette::Disabled, QPalette::Text, Qt::darkGray);
  palette.setColor(QPalette::Disabled, QPalette::ButtonText, Qt::darkGray);

  return palette;
}

QVulkanWindowRenderer *VulkanWindow::createRenderer()
{
  return new Renderer(this);
}
