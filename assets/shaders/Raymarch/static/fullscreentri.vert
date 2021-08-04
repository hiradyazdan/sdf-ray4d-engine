#version 450
//#extension GL_ARB_separate_shader_objects : enable

//layout(location = 0) in vec4 position;

out gl_PerVertex
{
  vec4 gl_Position;
};

//layout(push_constant) uniform VSConst
//{
//  mat4 mvp;
//} ubo;

vec2 positions[3] = vec2[](
    vec2(-1.0,  1.0),
    vec2(-1.0, -3.0),
    vec2( 3.0,  1.0)
);

void main()
{
    gl_Position = vec4(positions[gl_VertexIndex], 0.0, 1.0);
//  gl_Position = ubo.mvp * position;
}
