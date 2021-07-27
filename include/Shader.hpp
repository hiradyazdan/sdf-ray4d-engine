#pragma once

#include <QVulkanInstance>
#include <QFuture>

#include "Types.hpp"

namespace sdfRay4d
{
  using namespace vk;

  struct ShaderData
  {
    shader::Module shaderModule = VK_NULL_HANDLE;

    bool isValid() const { return shaderModule != VK_NULL_HANDLE; }
  };

  class Shader
  {
    public:
      void load(
        Device _device,
        QVulkanDeviceFunctions *_deviceFuncs,
        const QString &_filePath
      );
      ShaderData *getData();
      bool isValid() { return m_data.isValid(); }
      void reset();

    private:
      static QByteArray readShaderBinary(const QString &_filePath);
      static shader::StageFlags getShaderStage(const std::string &_fileExtension);

    private:
      bool m_maybeRunning = false;
      ShaderData m_data;

      QFuture<ShaderData> m_future;
  };
}
