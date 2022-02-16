#version 460 core

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec2 in_textureCoordinate;
layout(location = 2) in vec3 in_color;

layout(location = 0) out vec2 out_fragTextureCoordinate;
layout(location = 1) out vec3 out_color;

void main() {
  gl_Position = vec4(in_position.xy, 0.0, 1.0);
  out_fragTextureCoordinate = in_textureCoordinate;
  out_color = in_color;
}
