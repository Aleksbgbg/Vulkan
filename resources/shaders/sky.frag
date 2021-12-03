#version 450

layout(set = 1, binding = 0) uniform sampler2D textureSampler;

layout(location = 0) in vec2 in_fragTextureCoordinate;

layout(location = 0) out vec4 out_color;

void main() {
    out_color = texture(textureSampler, in_fragTextureCoordinate);
}
