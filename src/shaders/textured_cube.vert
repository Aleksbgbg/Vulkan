#version 450

layout(push_constant) uniform ModelViewTransformation {
  mat4 model;
  mat4 view;
} renderTransofrm;

layout(binding = 0) uniform ProjectionTransformation {
  mat4 value;
} projectionTransform;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec2 inTextureCoordinate;

layout(location = 0) out vec2 fragTextureCoordinate;

void main() {
  gl_Position =
    projectionTransform.value *
    renderTransofrm.view *
    renderTransofrm.model *
    vec4(inPosition, 1.0);
  fragTextureCoordinate = inTextureCoordinate;
}