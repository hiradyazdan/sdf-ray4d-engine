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

    private slots:
      void loadSDFGraph();
      void resizeSDFGraph(Qt::DockWidgetArea _area);
      void toggleSDFGraph();
      void toggleSDFGraphMenu(bool _isVisible);
      void loadAboutDialog();
      void autoCompileSDFGraph();
      void compileSDFGraph();
      void saveSDFNodes();
      void quitApp();

    private:
      void initVkInstance();
      void initVkLayers();
      void initVkWindow();

    private:
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
      VulkanWindow *m_vkWindow;

    /**
     * SDF Graph
     */
    private:
      SDFGraph *m_sdfGraph;

    /**
     * Qt Widgets
     */
    private:
      QWidget *m_vpWidget;
      QWidget *m_wrapperWidget;
      QWidget *m_mainMenuBar;

      QToolBar *m_sdfGraphToolbar;
      QDockWidget *m_sdfGraphWidget;
      QVBoxLayout *m_mainLayout;

    /**
     * Qt Widget Menus
     */
    private:
      QMenu *m_windowMenu;
      QMenu *m_sdfGraphMenu;
      QMenu *m_helpMenu;

    /**
     * Qt Widget Actions
     */
    private:
      QAction *m_loadSDFGraphAction;
      QAction *m_toggleSDFGraphAction;
      QAction *m_aboutAction;
      QAction *m_quitAction;
      QAction *m_autoCompileAction;
      QAction *m_compileAction;
      QAction *m_saveAction;

      QSize m_windowSize;
  };
}
