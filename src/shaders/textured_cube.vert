#version 450

layout(set = 0, binding = 0) uniform PerSceneData {
    mat4 projection;
} scene;

layout(set = 1, binding = 0) uniform PerFrameData {
    mat4 view;
} frame;

layout(push_constant) uniform PerObjectData {
    mat4 model;
} object;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec2 inTextureCoordinate;

layout(location = 0) out vec2 fragTextureCoordinate;

void main() {
    gl_Position =
        scene.projection *
        frame.view *
        object.model *
        vec4(inPosition, 1.0);
    fragTextureCoordinate = inTextureCoordinate;
}