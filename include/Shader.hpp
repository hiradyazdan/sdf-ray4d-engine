#pragma once

#include <QVulkanInstance>
#include <QFuture>

#include "Types.hpp"
#include "SPIRVCompiler.hpp"

namespace sdfRay4d
{
  using namespace vk;

  class Shader
  {
    public:
      Shader(
        const device::Device &_device,
        QVulkanDeviceFunctions *_deviceFuncs
      );

    public:
      struct Data
      {
        shader::Module shaderModule = VK_NULL_HANDLE;

        [[nodiscard]] bool isValid() const { return shaderModule != VK_NULL_HANDLE; }
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
      static shader::StageFlag getShaderStage(const std::string &_fileExtension);

      static void serializeVersionDirective(
        QByteArray &_rawBytes
      ) ;
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
      SPIRVCompiler m_spirvCompiler;

      bool m_isLoading = false;
      Data m_data;

    /**
     * Qt Vulkan Members
     */
    private:
      device::Device m_device = VK_NULL_HANDLE;
      QVulkanDeviceFunctions *m_deviceFuncs = VK_NULL_HANDLE;

      QFuture<Data> m_worker;

      QByteArray m_rawBytes;
      std::string m_template;
  };
}
