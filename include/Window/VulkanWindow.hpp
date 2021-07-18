#pragma once

#include <QVulkanWindow>

#include "Renderer.hpp"

namespace sdfRay4d
{
  using NodePtrSet = std::unordered_map<QUuid, std::shared_ptr<Node>>;

  class VulkanWindow : public QVulkanWindow
  {
    Q_OBJECT

    public:
      /**
       * @brief
       * @return
       */
      QVulkanWindowRenderer *createRenderer() override;
//      void updateShader(std::string &_shaderCode);
//
//      std::shared_ptr<ShaderManager> &getShaderMan() { return m_shaderMan; }
//      std::string &getShaderStart() { return m_shaderStart; }
//      std::string &getShaderEnd() { return m_shaderEnd; }

    signals:
      /**
       *
       * @param _nodes
       */
      void nodeEditorModified(sdfRay4d::NodePtrSet _nodes);

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

//    private:
//      std::shared_ptr<ShaderManager> m_shaderMan;
//      std::string m_shaderStart;
//      std::string m_shaderEnd;
  };
}
