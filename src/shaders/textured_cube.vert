#version 450

layout(set = 0, binding = 0) uniform ProjectionTransformation {
    mat4 projection;
} projectionTransform;

layout(set = 0, binding = 1) uniform ViewTransformation {
    mat4 view;
} viewTransform;

layout(push_constant) uniform ModelTransformation {
    mat4 model;
} modelTransform;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec2 inTextureCoordinate;

layout(location = 0) out vec2 fragTextureCoordinate;

void main() {
    gl_Position =
        projectionTransform.projection *
        viewTransform.view *
        modelTransform.model *
        vec4(inPosition, 1.0);
    fragTextureCoordinate = inTextureCoordinate;
}