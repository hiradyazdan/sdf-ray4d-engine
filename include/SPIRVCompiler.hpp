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

namespace sdfRay4d
{
  using namespace vk;

  class SPIRVCompiler
  {
    public:
      bool compile(
        shader::StageFlag _stage,
        const QByteArray &_glslSource,
        std::vector<std::uint32_t> &_spvBytes,
        std::string &_log,
        const std::string &_entryPoint = "main"
      ) noexcept;

    private:
      static EShLanguage getShaderLang(const shader::StageFlag &_stage) noexcept;
  };
}
