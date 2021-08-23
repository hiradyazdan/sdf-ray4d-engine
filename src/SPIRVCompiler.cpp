/*****************************************************
 * Partial Class: SPIRVCompiler (General)
 * Members: General Functions (Public/Private)
 *
 * This Class is split into partials to categorize
 * and classify the functionality
 * for the purpose of readability/maintainability
 *
 * The partials can be found in the respective
 * directory named as the class name
 *
 * Partials:
 * - compile_helpers.cpp
 *****************************************************/

#include "SPIRVCompiler.hpp"

using namespace sdfRay4d;

/**
 * @brief Compiles GLSL to SPIRV bytecode
 * @param[in]       _stage The Vulkan shader stage flag
 * @param[in]       _glslSource The GLSL source code to be compiled
 * @param[in,out]   _spvBytes The generated SPIRV code
 * @param[in,out]   _log Stores any log messages during the compilation process
 * @param[in]       _entryPoint The entrypoint function name of the shader stage
 * (as this has a default value with very rare use case, the exception was made
 * to position it as the last param after the out references)
 * @return boolean
 */
bool SPIRVCompiler::compile(
  shader::StageFlags _stage,
  const QByteArray &_glslSource,
  std::vector<std::uint32_t> &_spvBytes,
  std::string &_log,
  const std::string &_entryPoint
) noexcept
{
  glslang::InitializeProcess();

    auto messages = (EShMessages) (
        EShMsgDefault
      | EShMsgVulkanRules
      | EShMsgSpvRules
    );
    auto language = getShaderLang(_stage);
    auto source = std::string(_glslSource.begin(), _glslSource.end());
    const char *fileNames[1]  = { "" };
    const char *shaderSrc     = source.data();

    glslang::TShader shader(language);

    shader.setStringsWithLengthsAndNames(
      &shaderSrc,
      nullptr,
      fileNames,
      1
    );
    shader.setEntryPoint(_entryPoint.c_str());
    shader.setSourceEntryPoint(_entryPoint.c_str());

    if (
      !shader.parse(
        &glslang::DefaultTBuiltInResource, 100,
        false, messages
      )
    )
    {
      _log =  std::string(shader.getInfoLog()) + "\n" +
              std::string(shader.getInfoDebugLog());

      return false;
    }

    glslang::TProgram program;

    // Add shader to new program object.
    program.addShader(&shader);

    // Link program.
    if (!program.link(messages))
    {
      _log =  std::string(program.getInfoLog()) + "\n" +
              std::string(program.getInfoDebugLog());

      return false;
    }

    // Save any log that was generated.
    if (shader.getInfoLog())
    {
      _log += std::string(shader.getInfoLog()) + "\n" +
              std::string(shader.getInfoDebugLog()) + "\n";
    }

    if (program.getInfoLog())
    {
      _log += std::string(program.getInfoLog()) + "\n" +
              std::string(program.getInfoDebugLog());
    }

    // Translates to SPIRV
    auto intermediate = program.getIntermediate(language);

    if (!intermediate)
    {
      _log += "Failed to get shared intermediate code.\n";

      return false;
    }

    spv::SpvBuildLogger logger;

    glslang::GlslangToSpv(*intermediate, _spvBytes, &logger);

    _log += logger.getAllMessages() + "\n";

  // kill glslang process
  glslang::FinalizeProcess();

  return true;
}
