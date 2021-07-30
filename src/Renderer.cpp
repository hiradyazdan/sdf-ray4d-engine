/*****************************************************
 * Partial Class: Renderer (Ctor & General)
 * Members: General Functions (Public/Private)
 *
 * This Class is split into partials to categorize
 * and classify the functionality
 * for the purpose of readability/maintainability
 *
 * The partials can be found in the respective
 * directory named as the class name
 *****************************************************/

#include "Renderer.hpp"

using namespace sdfRay4d;

Renderer::Renderer(
  VulkanWindow *_vkWindow,
  bool _isMSAA
) :
  m_vkWindow(_vkWindow),
  m_isMSAA(_isMSAA)
{
  QObject::connect(
    &m_frameWatcher,
    &QFutureWatcherBase::finished,
    [this]
  {
    if (m_isFramePending)
    {
      m_isFramePending = false;
      m_vkWindow->frameReady();
      m_vkWindow->requestUpdate();
    }
  });
}

void Renderer::markViewProjDirty()
{
  m_vpDirty = m_vkWindow->concurrentFrameCount();
}

//ShaderModule Renderer::createShader(
//  const QString &_filePath,
//  ShaderStageFlags _stage
//)
//{
//  QFile file(_filePath);
//  if (!file.open(QIODevice::ReadOnly)) {
//    qWarning("Failed to read shader %s", qPrintable(_filePath));
//    return VK_NULL_HANDLE;
//  }
//  auto buffer = file.readAll();
//  file.close();
//
////  GLSLCompiler compiler;
////
////  std::vector<uint32_t> spirV;
////  std::string infoLog;
////
////  if(!compiler.compile_to_spirv(_stage, buffer, "main", spirV, infoLog))
////  {
////    qWarning("Failed to compile shader, {}", infoLog.c_str());
////
////    return VK_NULL_HANDLE;
////  }
//
//  ShaderModuleInfo shaderInfo;
//  memset(&shaderInfo, 0, sizeof(shaderInfo));
//  shaderInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
//  shaderInfo.codeSize = /*spirV*/buffer.size();
//  shaderInfo.pCode = reinterpret_cast<const uint32_t *>(buffer.constData());//spirV.data();
//
//  ShaderModule shaderModule;
//  Result err = m_deviceFuncs->vkCreateShaderModule(
//    m_vkWindow->device(),
//    &shaderInfo,
//    nullptr,
//    &shaderModule
//  );
//
//  if (err != VK_SUCCESS)
//  {
//    qWarning("Failed to create shader module: %d", err);
//    return VK_NULL_HANDLE;
//  }
//
//  return shaderModule;
//}
