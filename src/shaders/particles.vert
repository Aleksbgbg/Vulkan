#version 450

layout(set = 0, binding = 0) uniform PerFrameData {
    mat4 projection;
    mat4 view;
    vec3 cameraPosition;
    vec3 lightingPosition;
} frame;

struct ParticleRender {
    mat4 model;
    float fractionOfLife;
};

layout(set = 1, binding = 0) uniform ParticleRenderData {
    ParticleRender particleRender[1024];
} instance;

layout(location = 0) in vec3 in_position;

layout(location = 0) out vec3 out_color;

vec3 bias(vec3 a, vec3 b, float fraction) {
    float biasedFraction = pow(fraction, 1.0 / 2.0);
    return (a * biasedFraction) + (b * (1.0 - biasedFraction));
}

void main() {
    gl_Position =
        frame.projection *
        frame.view *
        instance.particleRender[gl_InstanceIndex].model *
        vec4(in_position, 1.0);

    out_color = bias(vec3(1.0, 0.0, 0.0), vec3(1.0, 0.404, 0.0), instance.particleRender[gl_InstanceIndex].fractionOfLife);
}