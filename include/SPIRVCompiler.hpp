#pragma once

#include <QVulkanWindow>
#include <QMutex>

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
      static bool compile(
        const shader::StageFlagBits &_stage,
        const QByteArray &_glslSource,
        std::vector<std::uint32_t> &_spvBytes,
        std::string &_log,
        const std::string &_entryPoint = "main"
      ) noexcept;

    private:
      static EShLanguage getShaderLang(const shader::StageFlagBits &_stage) noexcept;
  };
}
