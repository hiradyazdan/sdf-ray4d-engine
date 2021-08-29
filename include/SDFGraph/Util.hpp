#pragma once

namespace sdfRay4d::sdfGraph
{
  struct vec4
  {
    std::string x = "0.0000";
    std::string y = "0.0000";
    std::string z = "0.0000";
    std::string w = "1.0000";

    vec4(
      float _x,
      float _y,
      float _z,
      float _w
    ) :
    x(std::to_string(_x)),
    y(std::to_string(_y)),
    z(std::to_string(_z)),
    w(std::to_string(_w)) {}
  };
}
