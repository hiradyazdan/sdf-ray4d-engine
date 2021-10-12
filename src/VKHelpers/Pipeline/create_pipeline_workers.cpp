/*****************************************************
 * Partial Class: PipelineHelper
 * Members: Create Pipeline Workers (Public)
 *****************************************************/

#include "VKHelpers/Pipeline.hpp"

using namespace sdfRay4d::vkHelpers;

/**
 * @brief creates a single worker for each pipeline
 * i.e. multiple async pipelines
 * @param[in] _materials
 */
void PipelineHelper::createWorkers(
  const std::vector<MaterialPtr> &_materials
) noexcept
{
  m_materials = _materials;

  for(const auto &material : m_materials)
  {
    m_workers.push_back(
      QtConcurrent::run(
        this,
        &PipelineHelper::createPipeline,
        material
      )
    );
  }
}

/**
 * @brief creates a single worker for all pipelines
 * @param[in] _materials
 */
void PipelineHelper::createWorker(
  const std::vector<MaterialPtr> &_materials
) noexcept
{
  m_materials = _materials;

  m_inclusiveWorker = QtConcurrent::run(
    this,
    &PipelineHelper::createPipelines
  );
}

/**
 * @brief creates a single worker for only one pipeline
 * @param[in] _material
 * @param[in] _isHot
 */
void PipelineHelper::createWorker(
  const MaterialPtr &_material,
  bool _isHot
) noexcept
{
  m_isHot = _isHot;
  m_exclusiveWorker = QtConcurrent::run(
    this,
    &PipelineHelper::createPipeline,
    _material
  );
}
