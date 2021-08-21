#pragma once

#include <QVulkanWindow>

#include <nodes/Node>

#include "Renderer.hpp"

namespace sdfRay4d
{
  class Renderer;
  struct Material;

  class VulkanWindow : public QVulkanWindow
  {
    Q_OBJECT

    using NodePtrSet = std::unordered_map <QUuid, std::unique_ptr<QtNodes::Node>>;
    using MaterialPtr = std::shared_ptr<Material>;

    public:
      explicit VulkanWindow(bool _isDebug = false);

    public:
      /**
       * @brief
       * @return
       */
      QVulkanWindowRenderer *createRenderer() override;

    public:
      MaterialPtr &getSDFRMaterial(bool _isNew = false);
      void createSDFRPipeline();

    signals:
      /**
       *
       * @param[in] _nodes
       */
      void sdfGraphChanged(const sdfRay4d::VulkanWindow::NodePtrSet &_nodes);

    private:
      void mousePressEvent    (QMouseEvent *_event) override;
      void mouseReleaseEvent  (QMouseEvent *_event) override;
      void mouseMoveEvent     (QMouseEvent *_event) override;
      void wheelEvent         (QWheelEvent *_event) override;
      void keyPressEvent      (QKeyEvent   *_event) override;

    private:
      Renderer *m_renderer;
      bool m_isDebug;

      bool m_pressed = false;
      QPoint m_lastPos;
  };
}
