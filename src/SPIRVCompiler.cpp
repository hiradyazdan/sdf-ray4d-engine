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

//spvc::TargetLang        SPIRVCompiler::env_target_language         = spvc::TargetLang::EShTargetNone;
//spvc::TargetLangVersion SPIRVCompiler::env_target_language_version = (spvc::TargetLangVersion) 0; // SPIR-V 1.0
//
//void SPIRVCompiler::set_target_environment(
//  spvc::TargetLang target_language,
//  spvc::TargetLangVersion target_language_version
//)
//{
//  SPIRVCompiler::env_target_language         = target_language;
//  SPIRVCompiler::env_target_language_version = target_language_version;
//}
//
//void SPIRVCompiler::reset_target_environment()
//{
//  SPIRVCompiler::env_target_language         = spvc::TargetLang::EShTargetNone;
//  SPIRVCompiler::env_target_language_version = (spvc::TargetLangVersion) 0;
//}

/**
 * @brief Compiles GLSL to SPIRV bytecode
 * @param _stage The Vulkan shader stage flag
 * @param _glslSource The GLSL source code to be compiled
 * @param _entryPoint The entrypoint function name of the shader stage
 * @param[out] _spvBytes The generated SPIRV code
 * @param[out] _log Stores any log messages during the compilation process
 * @return
 */
bool SPIRVCompiler::compile(
  shader::StageFlags _stage,
  const QByteArray &_glslSource,
  const std::string &_entryPoint,
  std::vector<std::uint32_t> &_spvBytes,
  std::string &_log
)
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

    shader.setStringsWithLengthsAndNames(&shaderSrc, nullptr, fileNames, 1);
    shader.setEntryPoint(_entryPoint.c_str());
    shader.setSourceEntryPoint(_entryPoint.c_str());

  //  if (SPIRVCompiler::env_target_language != spvc::TargetLang::EShTargetNone)
  //  {
  //    shader.setEnvTarget(SPIRVCompiler::env_target_language, SPIRVCompiler::env_target_language_version);
  //  }

    if (
      !shader.parse(
        &glslang::DefaultTBuiltInResource,
        100,
        false,
        messages
      )
    )
    {
      _log = std::string(shader.getInfoLog()) + "\n" + std::string(shader.getInfoDebugLog());

      return false;
    }

    // Add shader to new program object.
    glslang::TProgram program;
    program.addShader(&shader);

    // Link program.
    if (!program.link(messages))
    {
      _log = std::string(program.getInfoLog()) + "\n" + std::string(program.getInfoDebugLog());

      return false;
    }

    // Save any log that was generated.
    if (shader.getInfoLog())
    {
      _log += std::string(shader.getInfoLog()) + "\n" + std::string(shader.getInfoDebugLog()) + "\n";
    }

    if (program.getInfoLog())
    {
      _log += std::string(program.getInfoLog()) + "\n" + std::string(program.getInfoDebugLog());
    }

    auto intermediate = program.getIntermediate(language);

    // Translate to SPIRV.
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
