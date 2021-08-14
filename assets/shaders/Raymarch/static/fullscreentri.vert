#version 450
//#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec4 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoords;

out gl_PerVertex
{
  vec4 gl_Position;
};

layout(location = 0) out vec3 vECVertNormal;
layout(location = 1) out vec3 vECVertPos;
layout(location = 2) out vec2 vECTexCoords;

//layout(push_constant) uniform VSConst
//{
//  mat4 mvp;
//} ubo;

//vec2 positions[3] = vec2[](
//    vec2(-1.0,  1.0),
//    vec2(-1.0, -3.0),
//    vec2( 3.0,  1.0)
//);

void main()
{
    gl_Position = position;//vec4(positions[gl_VertexIndex], 0.0, 1.0);
//  gl_Position = ubo.mvp * position;
}