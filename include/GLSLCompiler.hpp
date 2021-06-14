//#pragma once
//
//#include <string>
//#include <vector>
//#include <QVulkanWindow>
//
//#include <glslang/Public/ShaderLang.h>
//
//namespace ray4d
//{
//  /// Helper class to generate SPIRV code from GLSL source
//  /// A very simple version of the glslValidator application
//  class GLSLCompiler
//  {
//    public:
//      /**
//       * @brief Set the glslang target environment to translate to when generating code
//       * @param target_language The language to translate to
//       * @param target_language_version The version of the language to translate to
//       */
//      static void set_target_environment(
//        glslang::EShTargetLanguage target_language,
//        glslang::EShTargetLanguageVersion target_language_version
//      );
//
//      /**
//       * @brief Reset the glslang target environment to the default values
//       */
//      static void reset_target_environment();
//
//      /**
//       * @brief Compiles GLSL to SPIRV code
//       * @param stage The Vulkan shader stage flag
//       * @param glsl_source The GLSL source code to be compiled
//       * @param entry_point The entrypoint function name of the shader stage
//       * @param shader_variant The shader variant
//       * @param[out] spirv The generated SPIRV code
//       * @param[out] info_log Stores any log messages during the compilation process
//       */
//      bool compile_to_spirv(
//        VkShaderStageFlagBits stage,
//        const std::vector<uint8_t> &glsl_source,
//        const std::string &entry_point,
//        std::vector<std::uint32_t> &spirv,
//        std::string &info_log
//      );
//
//    private:
//      static glslang::EShTargetLanguage        env_target_language;
//      static glslang::EShTargetLanguageVersion env_target_language_version;
//  };
//}
