/*****************************************************
 * Partial Class: SPIRVCompiler
 * Members: Compile Helpers (Private)
 *****************************************************/

#include "SPIRVCompiler.hpp"

using namespace sdfRay4d;

/**
 *
 * @param _stage
 * @return
 */
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

//bool SPIRVCompiler::parseShader()
//{
//
//}




