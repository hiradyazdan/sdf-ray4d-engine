/*****************************************************
 * Partial Class: SPIRVCompiler
 * Members: Compile Helpers (Private)
 *****************************************************/

#include "SPIRVCompiler.hpp"

using namespace sdfRay4d;

/**
 *
 * @param[in] _stage
 * @returns EShLanguage
 */
EShLanguage SPIRVCompiler::getShaderLang(
  const shader::StageFlagBits &_stage
) noexcept
{
  switch (_stage)
  {
    case shader::StageFlag::VERTEX:                   return EShLangVertex;
    case shader::StageFlag::TESSELLATION_CONTROL:     return EShLangTessControl;
    case shader::StageFlag::TESSELLATION_EVALUATION:  return EShLangTessEvaluation;
    case shader::StageFlag::GEOMETRY:                 return EShLangGeometry;
    case shader::StageFlag::FRAGMENT:                 return EShLangFragment;
    case shader::StageFlag::COMPUTE:                  return EShLangCompute;

    case shader::StageFlag::RAYGEN_KHR:               return EShLangRayGen;
    case shader::StageFlag::ANY_HIT_KHR:              return EShLangAnyHit;
    case shader::StageFlag::CLOSEST_HIT_KHR:          return EShLangClosestHit;
    case shader::StageFlag::MISS_KHR:                 return EShLangMiss;
    case shader::StageFlag::INTERSECTION_KHR:         return EShLangIntersect;
    case shader::StageFlag::CALLABLE_KHR:             return EShLangCallable;

    default:                                          return EShLangVertex;
  }
}


