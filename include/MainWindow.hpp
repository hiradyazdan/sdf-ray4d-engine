#pragma once

#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QMessageBox>

#include <QVulkanWindow>

//#include "nodes/FlowView"

namespace ray4d
{
  class VulkanWindow : public QVulkanWindow
  {
    Q_OBJECT

    public:
      /**
       * @brief
       * @return
       */
      QVulkanWindowRenderer *createRenderer() override;

    signals:
      /**
       * @brief
       * @param _text {DESCRIPTION}
       */
      void vulkanInfoReceived(const QString &_text);
      /**
       * @brief
       * @param _colorValue {DESCRIPTION}
       */
      void frameQueued(int _colorValue);
  };

  class MainWindow : public QMainWindow
  {
    Q_OBJECT

    public:
      MainWindow();

    public:
      static QPalette setPalette();

    public slots:
      void onVulkanInfoReceived(const QString &text);
      void onFrameQueued(int colorValue);
      void onGrabRequested();

    private:
      void initVkInstance();
      void initVkWindow();
      void initWidgets();
      void initLayouts();

      void createActions();
      void createMenuBar();
      void createConnections();

    private:
      QVulkanInstance *m_vkInstance;

      QWidget *m_vpWidget;
      QWidget *m_wrapperWidget;

      QDockWidget *m_dockView;

      VulkanWindow *m_vkWindow;

      QVBoxLayout *m_mainLayout;

      QTabWidget *m_infoTab;
      QPlainTextEdit *m_info;

      QPushButton *m_grabBtn;
      QPushButton *m_quitBtn;
  };
}
