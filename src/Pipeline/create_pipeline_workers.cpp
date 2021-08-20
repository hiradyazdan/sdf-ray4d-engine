/*****************************************************
 * Partial Class: Pipeline
 * Members: Create Pipeline Workers (Public)
 *****************************************************/

#include "Pipeline.hpp"

using namespace sdfRay4d;

/**
 * @brief creates a single worker for each pipeline
 * i.e. multiple async pipelines
 * @param _materials
 */
void PipelineHelper::createWorkers(
  const std::vector<MaterialPtr> &_materials
)
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
 * @param _materials
 */
void PipelineHelper::createWorker(
  const std::vector<MaterialPtr> &_materials
)
{
  m_materials = _materials;

  m_inclusiveWorker = QtConcurrent::run(
    this,
    &PipelineHelper::createPipelines
  );
}

/**
 * @brief creates a single worker for only one pipeline
 * @param _material
 */
void PipelineHelper::createWorker(
  const MaterialPtr &_material,
  bool _isHot
)
{
  m_isHot = _isHot;
  m_exclusiveWorker = QtConcurrent::run(
    this,
    &PipelineHelper::createPipeline,
    _material
  );
}
