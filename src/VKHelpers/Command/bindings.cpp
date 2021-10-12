/*****************************************************
 * Partial Class: CommandHelper
 * Members: Command Buffer Bindings/Overloads (Private)
 *****************************************************/

#include "VKHelpers/Command.hpp"

using namespace sdfRay4d::vkHelpers;

/**
 *
 * @param[in] _material
 */
void CommandHelper::executeCmdBind(
  const MaterialPtr &_material
) noexcept
{
  m_deviceFuncs->vkCmdBindPipeline(
    m_cmdBuffer,
    VK_PIPELINE_BIND_POINT_GRAPHICS,
    _material->pipeline
  );

  device::Size vbOffset = 0;
  m_deviceFuncs->vkCmdBindVertexBuffers(
    m_cmdBuffer,
    0, 1,
    &_material->buffer,
    &vbOffset
  );

  const auto &descSets = _material->descSets;
  const auto &descSetCount = descSets.size();

  // the dynamic buffer points to the beginning of the vertex uniform data for the current frame.
  const uint32_t frameDynamicOffset = m_frameId * vbOffset;
  std::vector<uint32_t> frameDynamicOffsets = {}; // memset

  for(auto i = 0; i < descSetCount; i++)
  {
    for (auto j = 0; j < _material->dynamicDescCount; j++)
    {
      frameDynamicOffsets.push_back(frameDynamicOffset);
    }

    const auto dynamicOffsetCount = frameDynamicOffsets.size();

    m_deviceFuncs->vkCmdBindDescriptorSets(
      m_cmdBuffer,
      VK_PIPELINE_BIND_POINT_GRAPHICS,
      _material->pipelineLayout,
      i, descSetCount,
      &descSets[i],
      dynamicOffsetCount,
      dynamicOffsetCount > 0 ? frameDynamicOffsets.data() : nullptr
    );
  }
}
