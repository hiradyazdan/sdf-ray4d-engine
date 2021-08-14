/*****************************************************
 * Partial Class: Pipeline
 * Members: Create Pipeline Workers (Public)
 *****************************************************/

#include "Pipeline.hpp"

using namespace sdfRay4d;

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

void PipelineHelper::createWorker(
  const MaterialPtr &_material
)
{
  m_materials = { _material };

  m_workers.push_back(
    QtConcurrent::run(
      this,
      &PipelineHelper::createPipelines
    )
  );
}
