#pragma once

#include <string_view>
#include <utility>

namespace sdfRay4d::sdfGraph
{
  /**
   * @brief vec4 struct for glsl code
   * @note since the shader code translates from
   * string to glsl and then spirv, this custom
   * vec4 struct serves as a glsl vec4 construct
   */
  struct vec4
  {
    std::string x = "0.0";
    std::string y = "0.0";
    std::string z = "0.0";
    std::string w = "1.0";

    vec4(
      std::string _x,
      std::string _y,
      std::string _z,
      std::string _w
    ) :
    x(std::move(_x)),
    y(std::move(_y)),
    z(std::move(_z)),
    w(std::move(_w)) {}

//    vec4 operator +(const vec4 &_other)
//    {
//      x = _other.x;
//      y = _other.y;
//      z = _other.z;
//      w = _other.w;
//
//      return *this;
//    }

//    void operator =(const vec4 &_other) {
//      x = _other.x;
//      y = _other.y;
//      z = _other.z;
//      w = _other.w;
//    }
  };
}
