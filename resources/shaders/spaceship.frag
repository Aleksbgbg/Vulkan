#version 460 core

const uint MAX_LIGHTS = 128;

struct Material {
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
  float shininess;
};

struct Attenuation {
  float constant;
  float linear;
  float quadratic;
};

struct PointLight {
  vec3 position;

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;

  Attenuation attenuation;
};

layout(set = 0, binding = 0) uniform SceneUniform {
  mat4 view;
  mat4 projection;

  Material material;

  PointLight pointLights[MAX_LIGHTS];
  uint pointLightCount;
} scene;

layout(set = 1, binding = 1) uniform sampler2D textureSampler;
layout(set = 1, binding = 2) uniform sampler2D emissionSampler;

layout(location = 0) in vec3 in_fragPosition;
layout(location = 1) in vec3 in_normal;
layout(location = 2) in vec2 in_fragTextureCoordinate;

layout(location = 0) out vec4 out_color;

vec3 CalculatePointLight(PointLight light, Material material) {
  vec3 lightPositionView = vec3(scene.view * vec4(light.position, 1.0));

  // Ambient
  vec3 ambient = light.ambient * material.ambient;

  // Diffuse
  vec3 lightNormal = normalize(lightPositionView - in_fragPosition);
  vec3 surfaceNormal = normalize(in_normal);
  float diffuseStrength = max(dot(lightNormal, surfaceNormal), 0.0);
  vec3 diffuse = light.diffuse * diffuseStrength * material.diffuse;

  // Specular
  vec3 viewDirection = normalize(-in_fragPosition);
  vec3 halfwayDirection = normalize(lightNormal + viewDirection);
  float specularAngleStrength = max(dot(in_normal, halfwayDirection), 0.0);
  float specularity = pow(specularAngleStrength, material.shininess);
  vec3 specular = light.specular * specularity * material.specular;

  // Attentuation
  float distance = length(lightPositionView - in_fragPosition);
  float attentuation = 1.0 / (light.attenuation.constant + (light.attenuation.linear * distance) + (light.attenuation.quadratic * distance * distance));

  // Final color
  return (ambient + diffuse + specular) * attentuation;
}

void main() {
  vec3 lightColor = vec3(0.0);

  for (uint index = 0; index < scene.pointLightCount; ++index) {
    lightColor += CalculatePointLight(scene.pointLights[index], scene.material);
  }

  vec3 emission = texture(emissionSampler, in_fragTextureCoordinate).rgb;
  vec4 objectColor = texture(textureSampler, in_fragTextureCoordinate);

  out_color = vec4(lightColor + emission, 1.0) * objectColor;
}
