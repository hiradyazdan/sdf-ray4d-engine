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

  // Custom RenderPass
  // to mask out everything but depth attachment
  material->renderPass = m_pipelineHelper.getRenderPass(false);

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
}

void Renderer::initActorMaterial()
{
  auto &material = m_actorMaterial = std::make_shared<Material>(
    m_device,
    m_deviceFuncs
  );

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
}

void Renderer::initSDFRMaterial(const MaterialPtr &_material)
{
  _material->isHotSwappable = true;

  // default Qt Vulkan RenderPass
  _material->renderPass = m_pipelineHelper.getRenderPass();

  _material->createPushConstantRange(
    0,
    64
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
}

void Renderer::initSDFRMaterial()
{
  m_sdfrMaterial = std::make_shared<Material>(
    m_device,
    m_deviceFuncs
  );

  initSDFRMaterial(m_sdfrMaterial);
}
