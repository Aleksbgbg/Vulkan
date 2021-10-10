#version 450

layout(push_constant) uniform Highlight {
    layout(offset = 64) float multiplier;
} highlight;

layout(location = 0) in vec3 fragColor;

layout(location = 0) out vec4 outColor;

void main() {
    outColor = vec4(fragColor * highlight.multiplier, 1.0);
}