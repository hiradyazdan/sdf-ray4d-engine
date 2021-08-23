#pragma once

namespace sdfRay4d::sdfGraph
{
  struct vec4
  {
    float x = 0.0;
    float y = 0.0;
    float z = 0.0;
    float w = 1.0;

    vec4(
      float _x,
      float _y,
      float _z,
      float _w
    ) :
    x(_x),
    y(_y),
    z(_z),
    w(_w) {}
  };
}
