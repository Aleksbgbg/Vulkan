#version 460 core

layout(set = 1, binding = 0) uniform sampler2D textureSampler;

layout(location = 0) in vec2 in_fragTextureCoordinate;
layout(location = 1) in vec3 in_color;

layout(location = 0) out vec4 out_color;

void main() {
  float color = texture(textureSampler, in_fragTextureCoordinate).r;
  out_color = vec4(in_color * vec3(color), color);
}
