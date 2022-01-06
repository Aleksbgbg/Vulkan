#version 460 core

layout(set = 0, binding = 0) uniform GlobalRenderUniform {
  mat4 view;
  mat4 projection;
} scene;

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec2 in_textureCoordinate;

layout(location = 0) out vec2 out_fragTextureCoordinate;

void main() {
  vec4 framedView = scene.view * vec4(in_position, 0.0);
  framedView.w = 1.0;
  gl_Position = scene.projection * framedView;

  out_fragTextureCoordinate = in_textureCoordinate;
}
