#pragma once

#include <QVulkanWindow>

#include "nodeEditor_old/FlowScene.hpp"
#include "nodeEditor_old/FlowView.hpp"
#include "nodeEditor_old/Node.hpp"

#include "SDFGraph/DistanceFieldData.hpp"

#include "Renderer.hpp"
#include "ShaderManager.hpp"

namespace sdfRay4d
{
  class Renderer;

  using NodePtrSet = std::unordered_map<QUuid, std::shared_ptr<Node>>;

  class VulkanWindow : public QVulkanWindow
  {
    Q_OBJECT

    public:
      explicit VulkanWindow(bool _isDebug = false);

    public:
      /**
       * @brief
       * @return
       */
      QVulkanWindowRenderer *createRenderer() override;

    public:
//      std::shared_ptr<ShaderManager> &getShaderMan() { return m_shaderMan; }
//      std::string &getShaderStart() { return m_shaderStart; }
//      std::string &getShaderEnd() { return m_shaderEnd; }



    signals:
      /**
       *
       * @param _nodes
       */
      void nodeEditorModified(NodePtrSet _nodes);

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

//    public slots:
//      void onVulkanInfoReceived(const QString &text);
//      void onFrameQueued(int colorValue);
//      void onGrabRequested();

    private:
      void mousePressEvent(QMouseEvent *_event) override;
      void mouseReleaseEvent(QMouseEvent *_event) override;
      void mouseMoveEvent(QMouseEvent *_event) override;
      void wheelEvent(QWheelEvent *_event) override;
      void keyPressEvent(QKeyEvent *_event) override;

    private:
      Renderer *m_renderer;
      bool m_isDebug;

      bool m_pressed = false;
      QPoint m_lastPos;
//      std::shared_ptr<ShaderManager> m_shaderMan;
//      std::string m_shaderStart;
//      std::string m_shaderEnd;
  };
}
