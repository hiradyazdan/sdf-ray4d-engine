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
      Shader(
        Device &_device,
        QVulkanDeviceFunctions *_deviceFuncs,
        const QString &_shadersPath = QStringLiteral("shaders/"),
        int _version = 450
      );

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
        const QString &_filePath,
        const QStringList &_partialFilePaths = {}
      );
      void load(
        const std::string &_shaderData
      );

    public:
      void preload(
//        const QString &_replaceItem,
        const QStringList &_partialFilePaths
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

      void serializeVersionDirective(
        QByteArray &_rawBytes
      ) const;
      void serialize(
        const QByteArray &_rawBytes
      );
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
      Constants m_appConstants;

      const int m_version;
      bool m_isLoading = false;
      Data m_data;

    /**
     * Qt Vulkan Members
     */
    private:
      Device m_device = VK_NULL_HANDLE;
      QVulkanDeviceFunctions *m_deviceFuncs = VK_NULL_HANDLE;

      QFuture<Data> m_worker;
      QString m_shadersPath;

      QByteArray m_rawBytes;
  };
}
