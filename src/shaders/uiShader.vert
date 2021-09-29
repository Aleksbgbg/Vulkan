#version 450

layout(location = 0) in vec2 position;
layout(location = 1) in vec2 textureCoordinate;

layout(push_constant) uniform PushConstantsType {
  mat3 model;
  mat3 proj;
} transformation;

layout(location = 0) out vec2 fragTexCoord;

void main() {
  gl_Position = vec4(transformation.proj * transformation.model * vec3(position, 1.0), 1.0);
  fragTexCoord = textureCoordinate;
}