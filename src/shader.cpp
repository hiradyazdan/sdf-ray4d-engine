#include <QtConcurrentRun>
#include <QFile>
#include <QVulkanDeviceFunctions>

#include "Shader.hpp"
//#include "GLSLCompiler.hpp"

using namespace sdfRay4d;

void Shader::load(
  Device _device,
  QVulkanDeviceFunctions *_deviceFuncs,
  shader::StageFlags _stage,
  const QString &_filePath
)
{
  reset();

  m_maybeRunning = true;

  m_future = QtConcurrent::run([_device, _deviceFuncs, _stage, _filePath]()
  {
    ShaderData shaderData;
    QFile file(_filePath);

    if (!file.open(QIODevice::ReadOnly)) {
      qWarning("Failed to read shader %s", qPrintable(_filePath));
      return shaderData;
    }

    auto buffer = file.readAll();



//    GLSLCompiler compiler;
//
//    std::vector<uint32_t> spirV;
//    std::string infoLog;
//
//    if(!compiler.compile_to_spirv(_stage, buffer, "main", spirV, infoLog))
//    {
//      qWarning("Failed to compile shader, {}", infoLog.c_str());
//
//      return shaderData;
//    }



    shader::Info shaderInfo;
    memset(&shaderInfo, 0, sizeof(shaderInfo));
    shaderInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    shaderInfo.codeSize = /*spirV*/buffer.size();
    shaderInfo.pCode = /*spirV.data();*/reinterpret_cast<const uint32_t *>(buffer.constData());

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
