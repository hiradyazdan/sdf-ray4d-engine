/*****************************************************
 * Partial Class: Renderer
 * Members: Frame - User Input VKHelpers/Handlers (Private)
 *****************************************************/

#include "Renderer.hpp"

using namespace sdfRay4d;

void Renderer::yaw(float degrees)
{
  QMutexLocker locker(&m_guiMutex);
  m_camera.yaw(degrees);
  markViewProjDirty();
}

void Renderer::pitch(float degrees)
{
  QMutexLocker locker(&m_guiMutex);
  m_camera.pitch(degrees);
  markViewProjDirty();
}

void Renderer::walk(float amount)
{
  QMutexLocker locker(&m_guiMutex);
  m_camera.walk(amount);
  markViewProjDirty();
}

void Renderer::strafe(float amount)
{
  QMutexLocker locker(&m_guiMutex);
  m_camera.strafe(amount);
  markViewProjDirty();
}
