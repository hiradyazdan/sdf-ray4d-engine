/*****************************************************
 * Partial Class: Shader
 * Members: Load Function Overloads (Public/Private)
 *****************************************************/

#include <QtConcurrentRun>

#include "Shader.hpp"
#include "SPIRVCompiler.hpp"

using namespace sdfRay4d;

/**
 * PUBLIC
 *
 * @brief
 * Helper function to load a shader module asynchronously or
 * wait for the shader compilation to be finished, based on
 * type of the shader file
 *
 * if shader file is not spv, it can optionally serialize multiple
 * GLSL shader partial files (separate static shader instructions) into
 * a single bytecode array, compile it into SPIRV shader and finally
 * load it as a shader module
 * @param _device {Device} vulkan device
 * @param _deviceFuncs {QVulkanDeviceFunctions} qt vulkan device functions
 * @param _filePath {QString} main shader file path
 * @param _partialFilePaths {QStringList} shader's partial file paths (optional)
 *
 * TODO:
 * Make this function a variadic template with parameter pack
 * for filePaths. The complexity would be to make a separate
 * header file to have decl/definition in one file and also
 * the strings need to be either views or references so not to
 * copy over, initializer_list collection might be worth looking at.
 */
void Shader::load(
  Device &_device,
  QVulkanDeviceFunctions *_deviceFuncs,
  const QString &_filePath,
  const QStringList &_partialFilePaths
)
{
  reset();

  m_device = _device;
  m_deviceFuncs = _deviceFuncs;
  m_isLoading = true;

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
  m_future = QtConcurrent::run([fileExtension, _filePath, _partialFilePaths, this]()
  {
    auto isPrecompiled = fileExtension == "spv";
    auto rawBytes = getFileBytes(m_shadersPath + _filePath);

    std::vector<uint32_t> spvBytes;
    std::string log;

    if(isPrecompiled && !_partialFilePaths.isEmpty())
    {
      qWarning("\nWARNING: Partial files cannot be merged back into precompiled SPIRV files. Therefore, they're ignored\n");
    }

    if(!isPrecompiled)
    {
      // only if there is any partial shader helper file
      if(!_partialFilePaths.isEmpty())
      {
        serialize(_partialFilePaths, rawBytes);
      }

      /**
       * TODO: make SPIRVCompiler::compile thread-safe with lock/unlock?
       *
       * NOTE:
       *
       * SPV Compiler is not thread-safe and cannot synchronously compile
       * multiple shaders. It should only be called once per process, not per thread.
       */
      if(
        !SPIRVCompiler::compile(
          getShaderStage(fileExtension),
          rawBytes, "main", spvBytes, log
        )
      )
      {
        qWarning("Failed to compile shader: %s", log.c_str());
        return Data();
      }
    }

    return load(
      spvBytes, // runtime compiled spirv bytes (if available, otherwise empty)
      rawBytes, // pre-compiled spv file bytes
      isPrecompiled
    );
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
 * PRIVATE
 *
 * @brief Helper function to create/load shader module from the passed spirv bytecodes
 * @param _spvBytes {std::vector<uint32_t>} compiled spirv bytecodes at runtime
 * @param _rawSPVBytes {const QByteArray} pre-compiled spirv bytecodes from spv files
 * @param _isPrecompiled {bool}
 * @return data {Shader::Data} struct
 */
Shader::Data Shader::load(
  std::vector<uint32_t> &_spvBytes,
  const QByteArray &_rawSPVBytes,
  bool _isPrecompiled
)
{
  Data data;

  shader::Info shaderInfo = {};
  shaderInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
  shaderInfo.codeSize = !_isPrecompiled
                        ? _spvBytes.size() * sizeof(uint32_t)
                        : _rawSPVBytes.size();
  shaderInfo.pCode = !_isPrecompiled
                     ? _spvBytes.data()
                     : reinterpret_cast<const uint32_t *>(_rawSPVBytes.constData());

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
