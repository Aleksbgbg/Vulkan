#version 450

layout(set = 0, binding = 0) uniform PerFrameData {
    mat4 projection;
    mat4 view;
    vec3 cameraPosition;
    vec3 lightingPosition;
} frame;

layout(push_constant) uniform PerObjectData {
    mat4 model;
} object;

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec3 in_normal;
layout(location = 2) in vec2 in_textureCoordinate;

layout(location = 0) out vec3 out_fragPosition;
layout(location = 1) out vec3 out_normal;
layout(location = 2) out vec2 out_fragTextureCoordinate;
layout(location = 3) out vec3 out_lightingPosition;

void main() {
    vec4 viewSpacePosition = frame.view * object.model * vec4(in_position, 1.0);
    gl_Position = frame.projection * viewSpacePosition;

    out_fragPosition = vec3(viewSpacePosition);
    out_normal = vec3(transpose(inverse(frame.view * object.model)) * vec4(in_normal, 0.0));
    out_fragTextureCoordinate = in_textureCoordinate;
    out_lightingPosition = vec3(frame.view * vec4(frame.lightingPosition, 1.0));
}
