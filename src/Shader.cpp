#include <QtConcurrentRun>
#include <QFile>
#include <QVulkanDeviceFunctions>

#include "Shader.hpp"
#include "GLSLCompiler.hpp"

using namespace sdfRay4d;

shader::StageFlags Shader::getShaderStage(const std::string &_fileExtension)
{
  if (_fileExtension == "vert")
  {
    return VK_SHADER_STAGE_VERTEX_BIT;
  }
  else if (_fileExtension == "frag")
  {
    return VK_SHADER_STAGE_FRAGMENT_BIT;
  }
  else if (_fileExtension == "comp")
  {
    return VK_SHADER_STAGE_COMPUTE_BIT;
  }
  else if (_fileExtension == "geom")
  {
    return VK_SHADER_STAGE_GEOMETRY_BIT;
  }
  else if (_fileExtension == "tesc")
  {
    return VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
  }
  else if (_fileExtension == "tese")
  {
    return VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;
  }

  throw std::runtime_error("No Vulkan shader stage found for the file extension name.");
}

QByteArray Shader::readShaderBinary(const QString &_filePath)
{
  QFile file(_filePath);

  if (!file.open(QIODevice::ReadOnly))
  {
    qWarning("Failed to read shader %s", qPrintable(_filePath));
    return VK_NULL_HANDLE;
  }

  return file.readAll();
}

/**
 *
 * @brief Helper function to load a shader module asynchronously
 * @param _device
 * @param _deviceFuncs
 * @param _filePath
 */
void Shader::load(
  Device _device,
  QVulkanDeviceFunctions *_deviceFuncs,
  const QString &_filePath
)
{
  reset();

  m_maybeRunning = true;

  m_future = QtConcurrent::run([_device, _deviceFuncs, _filePath]()
  {
    ShaderData shaderData;

    auto buffer = readShaderBinary(_filePath);

    GLSLCompiler compiler;
    std::vector<uint32_t> spirV;
    std::string infoLog;

    std::string fileExtension = _filePath.toStdString();
    fileExtension = fileExtension.substr(fileExtension.find_last_of('.') + 1);
    auto isPrecompiled = fileExtension == "spv";

    /**
     * NOTE:
     * SPV Compiler cannot synchronously compile multiple shaders
     * It should only be called once per process, not per thread.
     */
    if(!isPrecompiled)
    {
      if(
        !compiler.compile_to_spirv(
          getShaderStage(fileExtension),
          buffer, "main", spirV, infoLog
        )
      )
      {
        qWarning("Failed to compile shader, Error: {}", infoLog.c_str());

        return shaderData;
      }
    }

    shader::Info shaderInfo;
    memset(&shaderInfo, 0, sizeof(shaderInfo));
    shaderInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    shaderInfo.codeSize = !isPrecompiled
      ? spirV.size() * sizeof(uint32_t)
      : buffer.size();
    shaderInfo.pCode = !isPrecompiled
      ? spirV.data()
      : reinterpret_cast<const uint32_t *>(buffer.constData());

    auto result = _deviceFuncs->vkCreateShaderModule(
      _device,
      &shaderInfo,
      nullptr,
      &shaderData.shaderModule
    );

    if (result != VK_SUCCESS)
    {
      qWarning("Failed to create shader module: %d", result);

      return shaderData;
    }

    return shaderData;
  });
}

ShaderData *Shader::getData()
{
  if (m_maybeRunning && !m_data.isValid())
  {
    m_data = m_future.result();
  }

  return &m_data;
}

void Shader::reset()
{
  m_data = ShaderData();
  m_maybeRunning = false;
}
