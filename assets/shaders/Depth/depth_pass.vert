#version 450

layout(location = 0) in vec4 position;
layout(location = 1) in vec3 normal;

out gl_PerVertex
{
  vec4 gl_Position;
};


layout(std140, binding = 0) uniform buf
{
  mat4 vp;
  mat4 model;
  mat3 modelNormal;
} ubuf;

void main()
{
  mat4 t = mat4(
    1, 0, 0, 0,
    0, 1, 0, 0,
    0, 0, 1, 0,
    0, 0, 0, 1
  );

  vec4 pos = ubuf.vp * t * ubuf.model * position;

  gl_Position = pos;
}
