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
 *****************************************************/

#include <iostream>

#include "SPIRVCompiler.hpp"

#include <glslang/SPIRV/GlslangToSpv.h>
#include <glslang/StandAlone/ResourceLimits.h>
#include <glslang/Include/ShHandle.h>

using namespace sdfRay4d;

EShLanguage SPIRVCompiler::getShaderLang(shader::StageFlags _stage)
{
  switch (_stage)
  {
    case VK_SHADER_STAGE_VERTEX_BIT:                  return EShLangVertex;
    case VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT:    return EShLangTessControl;
    case VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT: return EShLangTessEvaluation;
    case VK_SHADER_STAGE_GEOMETRY_BIT:                return EShLangGeometry;
    case VK_SHADER_STAGE_FRAGMENT_BIT:                return EShLangFragment;
    case VK_SHADER_STAGE_COMPUTE_BIT:                 return EShLangCompute;
    case VK_SHADER_STAGE_RAYGEN_BIT_KHR:              return EShLangRayGen;
    case VK_SHADER_STAGE_ANY_HIT_BIT_KHR:             return EShLangAnyHit;
    case VK_SHADER_STAGE_CLOSEST_HIT_BIT_KHR:         return EShLangClosestHit;
    case VK_SHADER_STAGE_MISS_BIT_KHR:                return EShLangMiss;
    case VK_SHADER_STAGE_INTERSECTION_BIT_KHR:        return EShLangIntersect;
    case VK_SHADER_STAGE_CALLABLE_BIT_KHR:            return EShLangCallable;
    default:                                          return EShLangVertex;
  }
}

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

bool SPIRVCompiler::compile(
  shader::StageFlags _stage,
  const QByteArray &_glslSource,
  const std::string &_entryPoint,
  std::vector<std::uint32_t> &_spvBytecode,
  std::string &_log
)
{
  glslang::InitializeProcess();

  auto messages = static_cast<EShMessages>(EShMsgDefault | EShMsgVulkanRules | EShMsgSpvRules);
  auto language = getShaderLang(_stage);
  auto source = std::string(_glslSource.begin(), _glslSource.end());

  const char *file_name_list[1] = { "" };
  const char *shader_source     = reinterpret_cast<const char *>(source.data());

  glslang::TShader shader(language);

  shader.setStringsWithLengthsAndNames(&shader_source, nullptr, file_name_list, 1);
  shader.setEntryPoint(_entryPoint.c_str());
  shader.setSourceEntryPoint(_entryPoint.c_str());

//  if (SPIRVCompiler::env_target_language != spvc::TargetLang::EShTargetNone)
//  {
//    shader.setEnvTarget(SPIRVCompiler::env_target_language, SPIRVCompiler::env_target_language_version);
//  }

	if (!shader.parse(&glslang::DefaultTBuiltInResource, 100, false, messages))
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

  // Save any info log that was generated.
  if (shader.getInfoLog())
  {
    _log += std::string(shader.getInfoLog()) + "\n" + std::string(shader.getInfoDebugLog()) + "\n";
  }

  if (program.getInfoLog())
  {
    _log += std::string(program.getInfoLog()) + "\n" + std::string(program.getInfoDebugLog());
  }

  glslang::TIntermediate *intermediate = program.getIntermediate(language);

  // Translate to SPIRV.
  if (!intermediate)
  {
    _log += "Failed to get shared intermediate code.\n";

    return false;
  }

  spv::SpvBuildLogger logger;

  glslang::GlslangToSpv(*intermediate, _spvBytecode, &logger);

  _log += logger.getAllMessages() + "\n";

  // Shutdown glslang library.
  glslang::FinalizeProcess();

  return true;
}
