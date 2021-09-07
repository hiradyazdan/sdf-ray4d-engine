#pragma once

#include <QtWidgets>

#include "Types.hpp"
#include "VulkanWindow.hpp"

namespace sdfRay4d
{
  class MainWindow : public QMainWindow
  {
    Q_OBJECT

    public:
      MainWindow();

    public:
      static QPalette setPalette();

    /**
     * SDF Graph Widget Window Connection Slots
     * -------------------------------------------------
     */
    private slots:
      void loadSDFGraph();
      void resizeSDFGraph(Qt::DockWidgetArea _area);
      void toggleSDFGraph();
      void toggleSDFGraphMenu(bool _isVisible);
      void toggleSDFGraphTopLevel();

    /**
     * SDF Graph Toolbar Button Slots
     * -------------------------------------------------
     */
    private slots:
      void autoCompileSDFGraph();
      void compileSDFGraph();
      void saveSDFNodes();

    /**
     * Main Menu Button Slots
     * -------------------------------------------------
     */
    private slots:
      void loadAboutDialog();
      void quitApp();

    private:
      void initVkInstance();
      void initVkLayers();
      void initVkWindow();

    private:
      void createSDFGraphActions();
      void createSDFGraphWidgetConnections();
      void createActions();
      void createMenus();

      void initWidgets();
      void initLayouts();

    private:
//      void mousePressEvent(QMouseEvent *_event) override;
//      void mouseReleaseEvent(QMouseEvent *_event) override;
//      void mouseMoveEvent(QMouseEvent *_event) override;
      void keyPressEvent(QKeyEvent *_event) override;
      void resizeEvent(QResizeEvent *_event) override;

    /**
     * Vulkan
     */
    private:
      QVulkanInstance *m_vkInstance = VK_NULL_HANDLE;
      VulkanWindow *m_vkWindow = VK_NULL_HANDLE;

    /**
     * SDF Graph
     */
    private:
      SDFGraph *m_sdfGraph = nullptr;

    /**
     * Qt Widgets
     */
    private:
      QWidget *m_vpWidget = nullptr;
      QWidget *m_wrapperWidget = nullptr;
      QWidget *m_mainMenuBar = nullptr;

      QToolBar *m_sdfGraphToolbar = nullptr;
      QDockWidget *m_sdfGraphWidget = nullptr;
      QVBoxLayout *m_mainLayout = nullptr;

    /**
     * Qt Widget Menus
     */
    private:
      QMenu *m_windowMenu = nullptr;
      QMenu *m_sdfGraphMenu = nullptr;
      QMenu *m_helpMenu = nullptr;

    /**
     * Qt Widget Actions
     */
    private:
      QAction *m_loadSDFGraphAction = nullptr;
      QAction *m_toggleSDFGraphAction = nullptr;
      QAction *m_aboutAction = nullptr;
      QAction *m_quitAction = nullptr;
      QAction *m_autoCompileAction = nullptr;
      QAction *m_compileAction = nullptr;
      QAction *m_saveAction = nullptr;

      QSize m_windowSize;
  };
}
