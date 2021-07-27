#version 410 core

layout(location = 0) in vec2 a_Position;
layout(location = 1) in vec2 a_FragCoord;
layout(location = 0) out vec2 o_FragCoord;

void main() {
  o_FragCoord = a_FragCoord;
  gl_Position = vec4(a_Position, 0.0, 1.0);
}
