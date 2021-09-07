/*****************************************************
 * Partial Class: Renderer
 * Members: Init Materials Helpers (Private)
 *****************************************************/

#include "Renderer.hpp"

using namespace sdfRay4d;

void Renderer::initDepthMaterial()
{
  auto &material = m_depthMaterial = std::make_shared<Material>(
    m_device,
    m_deviceFuncs
  );

  material->vertexCount = 2 * 3; // FIXME

  material->sourceStage =
      VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT
    | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
  material->destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;

  // Custom RenderPass
  // to mask out everything but depth attachment
  material->renderPass = m_pipelineHelper.getRenderPass(material->isDefault = false);

  // Note the std140 packing rules.
  // A vec3 still has an alignment of 16,
  // while a mat3 is like 3 * vec3.
  material->vertUniSize = setDynamicOffsetAlignment(
    2 * 64 + 48
  ); // depth_pass.vert

  // TODO : might not need this for depth texture
  const auto &maxSamplerAnisotropy = getDeviceLimits()->maxSamplerAnisotropy;
  material->texture.createSampler(maxSamplerAnisotropy);

  material->layoutBindings.resize(1);;

  material->layoutBindings[0] = {
    0, // binding
    VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, // descriptorType
    1, // descriptorCount
    VK_SHADER_STAGE_VERTEX_BIT, // stageFlags
    nullptr // pImmutableSamplers
  };
  material->descSetLayoutCount = 1;
  material->dynamicDescCount = 1;

  m_materials.push_back(material);
}

void Renderer::initActorMaterial()
{
  auto &material = m_actorMaterial = std::make_shared<Material>(
    m_device,
    m_deviceFuncs
  );

  material->vertexCount = m_actorMesh.data()->vertexCount;

  // default Qt Vulkan RenderPass
  material->renderPass = m_pipelineHelper.getRenderPass();

  // Note the std140 packing rules.
  // A vec3 still has an alignment of 16,
  // while a mat3 is like 3 * vec3.
  material->vertUniSize = setDynamicOffsetAlignment(
    2 * 64 + 48
  ); // see color_phong.vert
  material->fragUniSize = setDynamicOffsetAlignment(
    6 * 16 + 12 + 2 * 4
  ); // see color_phong.frag

  material->layoutBindings.resize(2);

  material->layoutBindings[0] = {
    0, // binding
    VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, // descriptorType
    1, // descriptorCount
    VK_SHADER_STAGE_VERTEX_BIT, // stageFlags
    nullptr // pImmutableSamplers
  };
  material->layoutBindings[1] = {
    1, // binding
    VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, // descriptorType
    1, // descriptorCount
    VK_SHADER_STAGE_FRAGMENT_BIT, // stageFlags
    nullptr // pImmutableSamplers
  };
  material->descSetLayoutCount = 1;
  material->dynamicDescCount = 2;

  m_materials.push_back(material);


//  if (m_vpDirty)
//  {
//    if (m_vpDirty) --m_vpDirty;
//
//    QMatrix4x4 vp, model;
//    QMatrix3x3 modelNormal;
//    QVector3D eyePos;
//    getMatrices(&vp, &model, &modelNormal, &eyePos);
//
//    // Map the uniform data for the current frame, ignore the geometry data at
//    // the beginning and the uniforms for other frames.
//    quint8 *p;
//    auto result = m_deviceFuncs->vkMapMemory(
//      m_device, m_bufferMemory,
//      material->uniMemStartOffset + m_vkWindow->currentFrame() * (
//        material->vertUniSize +
//        material->fragUniSize
//      ),
//      material->vertUniSize + material->fragUniSize,
//      0, reinterpret_cast<void **>(&p)
//    );
//    if (result != VK_SUCCESS)
//    {
//      qFatal("Failed to map memory: %d", result);
//    }
//
//    // Vertex shader uniforms
//    memcpy(p, vp.constData(), 64);
//    memcpy(p + 64, model.constData(), 64);
//    const float *mnp = modelNormal.constData();
//    memcpy(p + 128, mnp, 12);
//    memcpy(p + 128 + 16, mnp + 3, 12);
//    memcpy(p + 128 + 32, mnp + 6, 12);
//
//    // Fragment shader uniforms
//    p += material->vertUniSize;
//    writeFragUni(p, eyePos);
//
////    m_deviceFuncs->vkUnmapMemory(m_device, m_bufferMemory);
//  }
}

/**
 *
 * @param[in] _material
 */
void Renderer::initSDFRMaterial(const MaterialPtr &_material)
{
  _material->isHotSwappable = true;

  _material->vertexCount = 2 * 3;

  // default Qt Vulkan RenderPass
  _material->renderPass = m_pipelineHelper.getRenderPass();

  _material->createPushConstants(
    0, 64,
    {
      (float) m_vkWindow->width(),
      (float) m_vkWindow->height(),

      (float) 1,
      (float) 1,

      (float) 1,

      m_nearPlane,
      m_farPlane
    }
  );

  _material->vertUniSize = setDynamicOffsetAlignment(
    2 * 64 + 48
  ); // see color_phong.vert
  _material->fragUniSize = setDynamicOffsetAlignment(
    6 * 16 + 12 + 2 * 4
  ); // see color_phong.frag

  _material->layoutBindings.resize(1);

  //  _material->layoutBindings[0] = {
  //    0, // binding
  //    VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, // descriptorType
  //    1, // descriptorCount
  //    VK_SHADER_STAGE_VERTEX_BIT, // stageFlags
  //    nullptr // pImmutableSamplers
  //  };
  _material->layoutBindings[0] = {
    0, // binding
    VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, // descriptorType
    1, // descriptorCount
    VK_SHADER_STAGE_FRAGMENT_BIT, // stageFlags
    nullptr // pImmutableSamplers
  };
  _material->descSetLayoutCount = 1;
  _material->dynamicDescCount = 0;
}

void Renderer::initSDFRMaterial()
{
  m_sdfrMaterial = std::make_shared<Material>(
    m_device,
    m_deviceFuncs
  );

  initSDFRMaterial(m_sdfrMaterial);

  m_materials.push_back(m_sdfrMaterial);
}
