#pragma once

#include <QVulkanWindow>

#include "nodeEditor_old/FlowScene.hpp"
#include "nodeEditor_old/FlowView.hpp"
#include "nodeEditor_old/Node.hpp"

#include "SDFGraph/DistanceFieldData.hpp"

#include "Renderer.hpp"

namespace sdfRay4d
{
  class Renderer;
  struct Material;

  class VulkanWindow : public QVulkanWindow
  {
    Q_OBJECT

    using NodePtrSet = std::unordered_map
    <
      QUuid,
      std::shared_ptr<Node>
    >;
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
      void createSDFPipeline();
      void destroySDFPipeline();
      MaterialPtr getSDFMaterial();

    signals:
      /**
       *
       * @param _nodes
       */
      void sdfGraphChanged(sdfRay4d::VulkanWindow::NodePtrSet _nodes);

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
