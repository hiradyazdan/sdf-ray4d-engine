/*****************************************************
 * Partial Class: CommandHelper
 * Members: Frame Draw Commands/Overloads (Public)
 *****************************************************/

#include "Helpers/Command.hpp"

using namespace sdfRay4d::helpers;

/**
 *
 * @param[in] _material
 */
void CommandHelper::executeCmdDraw(
  const MaterialPtr &_material
) noexcept
{
  m_deviceFuncs->vkCmdDraw(
    m_cmdBuffer,
    _material->vertexCount,
    1,
    0, 0
  );
}
