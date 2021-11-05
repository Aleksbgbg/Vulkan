#version 450

layout(set = 0, binding = 0) uniform PerFrameData {
    mat4 projection;
    mat4 view;
    vec3 cameraPosition;
    vec3 lightingPosition;
} frame;

layout(location = 0) in vec3 in_position;

void main() {
    gl_Position = frame.projection * frame.view * vec4(in_position, 1.0);
}
