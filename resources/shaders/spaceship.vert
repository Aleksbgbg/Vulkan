#version 460 core

layout(set = 0, binding = 0) uniform SceneUniform {
  mat4 view;
  mat4 projection;
} scene;

layout(set = 1, binding = 0) uniform ModelTransformUniform {
  mat4 model;
} object;

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec3 in_normal;
layout(location = 2) in vec2 in_textureCoordinate;

layout(location = 0) out vec3 out_fragPosition;
layout(location = 1) out vec3 out_normal;
layout(location = 2) out vec2 out_fragTextureCoordinate;

void main() {
  vec4 viewSpacePosition = scene.view * object.model * vec4(in_position, 1.0);
  gl_Position = scene.projection * viewSpacePosition;

  out_fragPosition = vec3(viewSpacePosition);
  out_normal = vec3(transpose(inverse(scene.view * object.model)) * vec4(in_normal, 0.0));
  out_fragTextureCoordinate = in_textureCoordinate;
}
