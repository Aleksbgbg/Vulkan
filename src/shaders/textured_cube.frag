#version 450

layout(set = 1, binding = 0) uniform sampler2D textureSampler;

layout(push_constant) uniform Highlight {
    layout(offset = 64) float multiplier;
} highlight;

layout(location = 0) in vec2 fragTextureCoordinate;

layout(location = 0) out vec4 outColor;

void main() {
    outColor = texture(textureSampler, fragTextureCoordinate) * highlight.multiplier;
}