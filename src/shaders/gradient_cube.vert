#version 450

layout(binding = 0) uniform ProjectionTransformation {
    mat4 projection;
} projectionTransform;

layout(binding = 1) uniform ViewTransformation {
    mat4 view;
} viewTransform;

layout(push_constant) uniform ModelTransformation {
    mat4 model;
} modelTransform;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;

layout(location = 0) out vec3 fragColor;

void main() {
    gl_Position =
        projectionTransform.projection *
        viewTransform.view *
        modelTransform.model *
        vec4(inPosition, 1.0);
    fragColor = inColor;
}