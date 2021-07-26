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

    private:
      Renderer *m_renderer;
      bool m_isDebug;
//      std::shared_ptr<ShaderManager> m_shaderMan;
//      std::string m_shaderStart;
//      std::string m_shaderEnd;
  };
}
