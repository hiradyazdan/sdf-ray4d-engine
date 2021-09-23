/*****************************************************
 * Partial Class: Renderer
 * Members: Init Materials Helpers (Private)
 *****************************************************/

#include "Renderer.hpp"

using namespace sdfRay4d;

void Renderer::initDepthMaterial()
{
  auto &material = m_depthMaterial = std::make_shared<Mat>(
    m_device,
    m_deviceFuncs
  );

  material->vertexCount = m_actorMesh.data()->vertexCount; // FIXME
  material->bufferSize = material->vertexCount * 8 * sizeof(float); // FIXME
  material->bufferUsage =
      VK_BUFFER_USAGE_VERTEX_BUFFER_BIT // Vertex Buffer
    | VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT; // Uniform Buffer

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
    2 * 64 + 48 // FIXME
  ); // depth_pass.vert

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
  auto &material = m_actorMaterial = std::make_shared<Mat>(
    m_device,
    m_deviceFuncs
  );

  material->vertexCount = m_actorMesh.data()->vertexCount;
  material->bufferSize = material->vertexCount * 8 * sizeof(float);
  material->bufferUsage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT; // Vertex Buffer
  material->uniMemStartOffset = setDynamicOffsetAlignment(
    0 + material->memReq.size
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
  material->dynamicDescCount = 2;

  m_materials.push_back(material);
}

/**
 *
 * @param[in] _material
 */
void Renderer::initSDFRMaterial(const MaterialPtr &_material)
{
  _material->isHotSwappable = true;

  _material->vertexCount = 2 * 3;

  auto Kb = (1 << 10);
  auto UPLOAD_REGION_SIZE = 64 * Kb;
  _material->bufferSize = UPLOAD_REGION_SIZE;
  _material->bufferUsage =
      VK_BUFFER_USAGE_VERTEX_BUFFER_BIT
    | VK_BUFFER_USAGE_INDEX_BUFFER_BIT
    | VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT
    | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT
    | VK_BUFFER_USAGE_TRANSFER_SRC_BIT;

  // default Qt Vulkan RenderPass
  _material->renderPass = m_pipelineHelper.getRenderPass();

  _material->setPushConstantRange(0, 64);

  _material->vertUniSize = setDynamicOffsetAlignment(
    2 * 64 + 48
  ); // see rasterized_mesh_pass.vert
  _material->fragUniSize = setDynamicOffsetAlignment(
    6 * 16 + 12 + 2 * 4
  ); // see rasterized_mesh_pass.frag

  // TODO : might not need this for depth texture
  const auto &maxSamplerAnisotropy = getDeviceLimits()->maxSamplerAnisotropy;
  _material->texture.createSampler(maxSamplerAnisotropy);

  _material->layoutBindings.resize(1);

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
  m_sdfrMaterial = std::make_shared<Mat>(
    m_device,
    m_deviceFuncs
  );

  initSDFRMaterial(m_sdfrMaterial);

  m_materials.push_back(m_sdfrMaterial);
}
