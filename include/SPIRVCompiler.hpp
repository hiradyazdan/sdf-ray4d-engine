#pragma once

#include <iostream>
#include <string>
#include <vector>

#include <QVulkanWindow>

#include <glslang/Public/ShaderLang.h>
#include <glslang/SPIRV/GlslangToSpv.h>
#include <glslang/StandAlone/ResourceLimits.h>
#include <glslang/Include/ShHandle.h>

#include "Types.hpp"

//namespace sdfRay4d::spvc
//{
//  using TargetLang = glslang::EShTargetLanguage;
//  using TargetLangVersion = glslang::EShTargetLanguageVersion;
//}

namespace sdfRay4d
{
  using namespace vk;

  /// Helper class to generate SPIRV code from GLSL source
  /// A very simple version of the glslValidator application
  class SPIRVCompiler
  {
    public:
      /**
       * @brief Set the glslang target environment to translate to when generating code
       * @param target_language The language to translate to
       * @param target_language_version The version of the language to translate to
       */
//      static void set_target_environment(
//        spvc::TargetLang target_language,
//        spvc::TargetLangVersion target_language_version
//      );

      /**
       * @brief Reset the glslang target environment to the default values
       */
//      static void reset_target_environment();

      static bool compile(
        shader::StageFlags _stage,
        const QByteArray &_glslSource,
        const std::string &_entryPoint,
        std::vector<std::uint32_t> &_spvBytecode,
        std::string &_log
      );

    private:
      static EShLanguage getShaderLang(shader::StageFlags _stage);

//    private:
//      static spvc::TargetLang        env_target_language;
//      static spvc::TargetLangVersion env_target_language_version;

    private:

  };
}
