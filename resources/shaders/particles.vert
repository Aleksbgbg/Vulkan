#version 450

layout(set = 0, binding = 0) uniform SceneUniform {
  mat4 view;
  mat4 projection;
} scene;

struct ParticleRender {
  mat4 model;
  float fractionOfLife;
};

layout(set = 1, binding = 0) buffer InstanceUniform {
  ParticleRender particleRenders[];
} instance;

layout(location = 0) in vec3 in_position;

layout(location = 0) out vec3 out_color;

vec3 bias(vec3 a, vec3 b, float fraction) {
  float biasedFraction = pow(fraction, 1.0 / 2.0);
  return (a * biasedFraction) + (b * (1.0 - biasedFraction));
}

void main() {
  gl_Position =
  scene.projection *
  scene.view *
  instance.particleRenders[gl_InstanceIndex].model *
  vec4(in_position, 1.0);

  out_color = bias(vec3(1.0, 0.0, 0.0), vec3(1.0, 0.404, 0.0), instance.particleRenders[gl_InstanceIndex].fractionOfLife);
}
