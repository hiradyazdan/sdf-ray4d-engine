#pragma once

#include <vector>

#include <QVulkanInstance>
#include <QFuture>

#include "Types.hpp"

namespace sdfRay4d
{
  using namespace vk;

  class Shader
  {
    public:
      Shader();

    public:
      struct Data
      {
        shader::Module shaderModule = VK_NULL_HANDLE;

        bool isValid() const { return shaderModule != VK_NULL_HANDLE; }
      };

    /**
     * Load Function Overloads (Public)
     * -------------------------------------------------
     *
     */
    public:
      void load(
        Device &_device,
        QVulkanDeviceFunctions *_deviceFuncs,
        const QString &_filePath,
        const QStringList &_partialFilePaths = {}
      );
    /**
     * General Helpers
     * -------------------------------------------------
     *
     */
    public:
      Data *getData();
      bool isValid();
      void reset();

    /**
     * Load Function Helpers
     * -------------------------------------------------
     *
     */
    private:
      static QByteArray getFileBytes(const QString &_filePath);
      static shader::StageFlags getShaderStage(const std::string &_fileExtension);

      void serialize(
        const QStringList &_partialFilePaths,
        QByteArray &_rawBytes
      );

    /**
     * Load Overloads (Private)
     * -------------------------------------------------
     *
     */
    private:
      Data load(
        std::vector<uint32_t> &_spvBytes,
        const QByteArray &_rawSPVBytes = nullptr,
        bool _isPrecompiled = false
      );

    private:
      bool m_isLoading = false;
      Data m_data;

    /**
     * Qt Vulkan Members
     */
    private:
      Device m_device = VK_NULL_HANDLE;
      QVulkanDeviceFunctions *m_deviceFuncs = VK_NULL_HANDLE;
      QFuture<Data> m_future;
      QString m_shadersPath;
  };
}
