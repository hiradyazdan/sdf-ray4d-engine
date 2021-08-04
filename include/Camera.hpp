#pragma once

#include <QVector3D>
#include <QMatrix4x4>

namespace sdfRay4d
{
  class Camera
  {
    public:
      Camera();

    public:
      void yaw(float _degrees);
      void pitch(float _degrees);
      void walk(float _amount);
      void strafe(float _amount);

      QMatrix4x4 viewMatrix() const;

    private:
      QVector3D m_forward;
      QVector3D m_right;
      QVector3D m_up;
      QVector3D m_pos;

      QMatrix4x4 m_yawMatrix;
      QMatrix4x4 m_pitchMatrix;

      float m_yaw;
      float m_pitch;
  };
}
