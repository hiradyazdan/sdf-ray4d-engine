#pragma once

#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QMessageBox>

#include "Types.hpp"
#include "VulkanWindow.hpp"
#include "ShaderManager.hpp"

namespace sdfRay4d
{
  using NodePtrSet = std::unordered_map<QUuid, std::shared_ptr<Node>>;

  class MainWindow : public QMainWindow
  {
    Q_OBJECT

    public:
      MainWindow();

    public:
      static QPalette setPalette();

    public:
      NodePtrSet getNodes() { return m_sdfGraphScene->getNodes(); }

    public slots:
      void onVulkanInfoReceived(const QString &text);
      void onFrameQueued(int colorValue);
      void onGrabRequested();

      virtual void onNodeChanged(const sdfRay4d::NodePtrSet &_nodes);

    private:
      void mousePressEvent(QMouseEvent *_event) override;
      void mouseReleaseEvent(QMouseEvent *_event) override;
      void mouseMoveEvent(QMouseEvent *_event) override;
      void keyPressEvent(QKeyEvent *_event) override;

    private:
      void initVkInstance();
      void initVkLayers();
      void initVkWindow();

      void registerModels();
      void initSDFGraph();
      void setupOutputNode();

      void initWidgets();
      void initLayouts();

      std::string recurseNodeTree(
        const std::shared_ptr<Node> &_node,
        Mat4f _t,
        PortIndex portIndex = 0,
        unsigned int _cp = 0
      );
//
//      void createActions();
//      void createMenuBar();
//      void createConnections();

    private:
      /**
       * Vulkan
       */
      QVulkanInstance *m_vkInstance;
      VulkanWindow *m_vkWindow;

      /**
       * SDF Graph
       */
      FlowScene *m_sdfGraphScene;
      FlowView *m_sdfGraphView;

      Node *m_outputNode;

      std::shared_ptr<ShaderManager> m_shaderMan;
      std::string m_shaderStart;
      std::string m_shaderEnd;

      /**
       * Qt Widgets
       */
      QWidget *m_vpWidget;
      QWidget *m_wrapperWidget;

      QDockWidget *m_dockWidget;

      QVBoxLayout *m_mainLayout;

      QTabWidget *m_infoTab;
      QPlainTextEdit *m_info;

      QPushButton *m_grabBtn;
      QPushButton *m_quitBtn;

      bool m_pressed = false;
      QPoint m_lastPos;
  };
}
