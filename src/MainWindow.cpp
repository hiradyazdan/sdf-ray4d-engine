#include <fstream>

#include <QPointer>
#include <QLibraryInfo>

#include "Window/MainWindow.hpp"

using namespace sdfRay4d;

//Q_LOGGING_CATEGORY(lcVk, "qt.vulkan")

//static QPointer<QPlainTextEdit> messageLogWidget;
//static QtMessageHandler oldMessageHandler = nullptr;
//
//static void messageHandler(QtMsgType _msgType, const QMessageLogContext &_logContext, const QString &_text)
//{
//  if (!messageLogWidget.isNull()) messageLogWidget->appendPlainText(_text);
//  if (oldMessageHandler) oldMessageHandler(_msgType, _logContext, _text);
//}

MainWindow::MainWindow() :
m_shaderMan(ShaderManager::instance()),
m_outputNode(nullptr)
{
//  messageLogWidget = new QPlainTextEdit(QLatin1String(QLibraryInfo::build()) + QLatin1Char('\n'));
//  messageLogWidget->setReadOnly(true);
//
//  oldMessageHandler = qInstallMessageHandler(messageHandler);

  initVkWindow();
  initSDFGraph();

  initWidgets();
  initLayouts();

  std::ifstream s("shaders/shader.begin.frag");
  std::ifstream e("shaders/shader.end.frag");
  m_shaderStart = std::string((std::istreambuf_iterator<char>(s)), std::istreambuf_iterator<char>());
  m_shaderEnd = std::string((std::istreambuf_iterator<char>(e)), std::istreambuf_iterator<char>());

  connect(
    m_vkWindow, &VulkanWindow::nodeEditorModified, // signal/sender (event)
    this, &MainWindow::onNodeChanged // slot/receiver (event handler)
  );

//  connect(m_vkWindow, &VulkanWindow::vulkanInfoReceived, this, &MainWindow::onVulkanInfoReceived);
//  connect(m_vkWindow, &VulkanWindow::frameQueued, this, &MainWindow::onFrameQueued);
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
