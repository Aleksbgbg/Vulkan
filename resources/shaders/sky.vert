#version 450
#extension GL_EXT_debug_printf : enable

layout(set = 0, binding = 0) uniform PerFrameData {
    mat4 projection;
    mat4 view;
    vec3 cameraPosition;
    vec3 lightingPosition;
} frame;

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec2 in_textureCoordinate;

layout(location = 0) out vec2 out_fragTextureCoordinate;

void main() {
    vec4 framedView = frame.view * vec4(in_position, 0.0);
    framedView.w = 1.0;
    gl_Position = frame.projection * framedView;

    out_fragTextureCoordinate = in_textureCoordinate;
}
