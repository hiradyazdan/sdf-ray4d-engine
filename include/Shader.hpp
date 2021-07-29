#pragma once

#include <QVulkanInstance>
#include <QFuture>

#include "Types.hpp"

namespace sdfRay4d
{
  using namespace vk;

  class Shader
  {
    public:
      struct Data
      {
        shader::Module shaderModule = VK_NULL_HANDLE;

        bool isValid() const { return shaderModule != VK_NULL_HANDLE; }
      };

    public:
      void load(
        Device &_device,
        QVulkanDeviceFunctions *_deviceFuncs,
        const QString &_filePath
      );
      Data *getData();
      bool isValid() { return m_data.isValid(); }
      void reset();

    private:
      static QByteArray readShaderBinary(const QString &_filePath);
      static shader::StageFlags getShaderStage(const std::string &_fileExtension);

    private:
      Data load(
        std::vector<uint32_t> &_spv,
        const QByteArray &_buffer,
        bool _isPrecompiled = false
      );

    private:
      bool m_maybeRunning = false;

      Device m_device = VK_NULL_HANDLE;
      QVulkanDeviceFunctions *m_deviceFuncs = VK_NULL_HANDLE;
      Data m_data;

      QFuture<Data> m_future;
  };
}
