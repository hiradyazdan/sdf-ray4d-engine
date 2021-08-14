/*****************************************************
 * Class: Camera (Ctor & General)
 * Members: General Functions (Public)
 * Partials: None
 *****************************************************/

#include "Camera.hpp"

using namespace sdfRay4d;

Camera::Camera()
  : m_forward(0.0f, 0.0f, -1.0f),
    m_right(1.0f, 0.0f, 0.0f),
    m_up(0.0f, 1.0f, 0.0f),
    m_pos(QVector3D(0.0f, 0.0f, 20.0f)),
    m_yaw(0.0f),
    m_pitch(0.0f)
{}

static inline void clamp360(float *v)
{
  if (*v > 360.0f)  *v -= 360.0f;
  if (*v < -360.0f) *v += 360.0f;
}

void Camera::yaw(float degrees)
{
  m_yaw += degrees;
  clamp360(&m_yaw);
  m_yawMatrix.setToIdentity();
  m_yawMatrix.rotate(m_yaw, 0, 1, 0);

  QMatrix4x4 rotMat = m_pitchMatrix * m_yawMatrix;
  m_forward = (QVector4D(0.0f, 0.0f, -1.0f, 0.0f) * rotMat).toVector3D();
  m_right = (QVector4D(1.0f, 0.0f, 0.0f, 0.0f) * rotMat).toVector3D();
}

void Camera::pitch(float degrees)
{
  m_pitch += degrees;
  clamp360(&m_pitch);
  m_pitchMatrix.setToIdentity();
  m_pitchMatrix.rotate(m_pitch, 1, 0, 0);

  QMatrix4x4 rotMat = m_pitchMatrix * m_yawMatrix;
  m_forward = (QVector4D(0.0f, 0.0f, -1.0f, 0.0f) * rotMat).toVector3D();
  m_up = (QVector4D(0.0f, 1.0f, 0.0f, 0.0f) * rotMat).toVector3D();
}

void Camera::walk(float amount)
{
  m_pos[0] += amount * m_forward.x();
  m_pos[2] += amount * m_forward.z();
}

void Camera::strafe(float amount)
{
  m_pos[0] += amount * m_right.x();
  m_pos[2] += amount * m_right.z();
}

QMatrix4x4 Camera::viewMatrix() const
{
  auto &&matrix = m_pitchMatrix * m_yawMatrix;

  matrix.translate(-m_pos);

  return matrix;
}
