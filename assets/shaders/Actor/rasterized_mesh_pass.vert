#version 450

layout(location = 0) in vec4 position;
layout(location = 1) in vec3 normal;
//layout(location = 2) in vec2 texCoord;

out gl_PerVertex {
  vec4 gl_Position;
};

layout(location = 0) out vec3 vECVertNormal;
layout(location = 1) out vec3 vECVertPos;
layout(location = 2) out vec2 vECTexCoords;

layout(std140, binding = 0) uniform buf {
  mat4 vp;
  mat4 model;
  mat3 modelNormal;
} ubuf;

void main()
{
  vECVertNormal = normalize(ubuf.modelNormal * normal);
  mat4 t = mat4(
    1, 0, 0, 0,
    0, 1, 0, 0,
    0, 0, 1, 0,
    0, 0, 0, 1
  );
  vECVertPos = vec3(t * ubuf.model * position);

  gl_Position = ubuf.vp * t * ubuf.model * position;

  vECTexCoords = vec2((gl_Position.z/gl_Position.w+1.0)/2);
}
