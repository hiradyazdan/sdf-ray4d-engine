#include <QtConcurrentRun>

#include "Shader.hpp"
#include "SPIRVCompiler.hpp"

using namespace sdfRay4d;

/**
 *
 * @brief
 * Helper function to load a shader module asynchronously or
 * wait for the shader compilation to be finished, based on
 * type of the shader file
 * @param _device {Device}
 * @param _deviceFuncs {QVulkanDeviceFunctions}
 * @param _filePath {QString}
 */
void Shader::load(
  Device &_device,
  QVulkanDeviceFunctions *_deviceFuncs,
  const QString &_filePath
)
{
  reset();

  m_device = _device;
  m_deviceFuncs = _deviceFuncs;
  m_maybeRunning = true;

  auto fileExtension = _filePath.toStdString();
  fileExtension = fileExtension.substr(fileExtension.find_last_of('.') + 1);

  /**
   * NOTE:
   *
   * As compiling the shader cannot be done asynchronously,
   * there could be two ways of implementing this:
   *
   * - separate the compilation functionality to be outside the async callback function
   *   and only load shader module async without the need to explicitly make it wait to be completed
   *
   * - put both compilation and loading the shader module inside the async callback function
   *   and make the thread to explicitly wait for the job to be completed.
   *
   * In both methods, the shader compilation will block the process to be finished and there will be
   * no extra runtime performance penalty using one over another.
   *
   * Explicit wait function may only slightly increase the length of the code path and
   * therefore increase the build time very slightly (which is already optimized by cmake's UNITY_BUILD/PCH).
   *
   * So, for the sake of code readability/maintainability, it may be cleaner to put everything inside the async
   * callback function.
   */
  m_future = QtConcurrent::run([fileExtension, _filePath, this]()
  {
    auto isPrecompiled = fileExtension == "spv";
    auto buffer = readShaderBinary(_filePath);

    std::vector<uint32_t> spv;
    std::string log;

    /**
    * NOTE:
    *
    * SPV Compiler cannot synchronously compile multiple shaders
    * It should only be called once per process, not per thread.
    */
    if(!isPrecompiled &&
       !SPIRVCompiler::compile(
         getShaderStage(fileExtension),
         buffer, "main", spv, log
       )
    )
    {
      qWarning("Failed to compile shader, Error: {}", log.c_str());
      return Data();
    }

    return load(spv, buffer, isPrecompiled);
  });

  /**
   * This will avoid triggering synchronous spirv compilation
   * as it will crash the app otherwise. This is due to the
   * glslang compiler initialize call being only per process
   * and not per thread.
   */
  if(fileExtension != "spv")
  {
    // equivalent to std::future::wait_for with std::future_status::ready
    m_future.waitForFinished();
  }
}

/**
 * @brief Helper function to create/load shader module
 * @param _spv {std::vector<uint32_t>}
 * @param _buffer {const QByteArray}
 * @param _isPrecompiled {bool}
 * @return data {Shader::Data} struct
 */
Shader::Data Shader::load(
  std::vector<uint32_t> &_spv,
  const QByteArray &_buffer,
  bool _isPrecompiled
)
{
  Data data;

  shader::Info shaderInfo;
  memset(&shaderInfo, 0, sizeof(shaderInfo));
  shaderInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
  shaderInfo.codeSize = !_isPrecompiled
                        ? _spv.size() * sizeof(uint32_t)
                        : _buffer.size();
  shaderInfo.pCode = !_isPrecompiled
                     ? _spv.data()
                     : reinterpret_cast<const uint32_t *>(_buffer.constData());

  auto result = m_deviceFuncs->vkCreateShaderModule(
    m_device,
    &shaderInfo,
    nullptr,
    &data.shaderModule
  );

  if (result != VK_SUCCESS)
  {
    qWarning("Failed to create shader module: %d", result);

    return data;
  }

  return data;
}
