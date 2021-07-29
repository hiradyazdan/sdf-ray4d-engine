/*****************************************************
 * Partial Class: Renderer
 * Members: Resources Methods Overrides (Public)
 *****************************************************/

#include "Renderer.hpp"

using namespace sdfRay4d;

void Renderer::preInitResources()
{
  if(m_isMSAA)
  {
    const auto sampleCounts = m_vkWindow->supportedSampleCounts();
    qDebug() << "Supported sample counts:" << sampleCounts;

    for (int s = 16; s >= 4; s /= 2)
    {
      if (sampleCounts.contains(s))
      {
        qDebug("Requesting sample count %d", s);
        m_vkWindow->setSampleCount(s);

        break;
      }
    }
  }
}

void Renderer::initResources()
{
  qDebug("initResources");

  m_isFramePending = false;

  m_vkInstance  = m_vkWindow->vulkanInstance();
  m_device      = m_vkWindow->device();
  m_deviceFuncs = m_vkInstance->deviceFunctions(m_device);

//  vma::AllocatorInfo allocatorInfo;
//  allocatorInfo.vulkanApiVersion  = VK_API_VERSION_1_2;
//  allocatorInfo.physicalDevice    = m_vkWindow->physicalDevice();
//  allocatorInfo.device            = m_device;
//  allocatorInfo.instance          = m_vkInstance->vkInstance();
//
//  vmaCreateAllocator(&allocatorInfo, &m_allocator);

  loadShaders();
  m_pipelinesFuture = QtConcurrent::run(this, &Renderer::createPipelines);
}

void Renderer::loadShaders()
{
  if (!m_objMaterial.vertexShader.isValid())
  {
    m_objMaterial.vertexShader.load(
      m_device,
      m_deviceFuncs,
      "assets/shaders/SDF/static/fullscreentri.vert.spv"
    );
  }

  if (!m_objMaterial.fragmentShader.isValid())
  {
    m_objMaterial.fragmentShader.load(
      m_device,
      m_deviceFuncs,
      "assets/shaders/SDF/static/rtprimitives.frag.spv"
    );
  }
}

void Renderer::releaseResources()
{
  qDebug("releaseResources");

  m_pipelinesFuture.waitForFinished();

  destroyDescriptorSets();
  destroyPipeline();
  destroyBuffers();
  destroyShaderModules();
}
