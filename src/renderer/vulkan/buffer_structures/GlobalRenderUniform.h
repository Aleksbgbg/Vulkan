#ifndef SRC_RENDERER_VULKAN_BUFFER_STRUCTURES_GLOBALRENDERUNIFORM_H_
#define SRC_RENDERER_VULKAN_BUFFER_STRUCTURES_GLOBALRENDERUNIFORM_H_

#include "core/types.h"
#include "util/include/glm.h"

inline constexpr u32 MAX_LIGHTS = 128;

struct GlobalRenderUniform {
  struct Material {
    alignas(16) glm::vec3 ambient;
    alignas(16) glm::vec3 diffuse;
    alignas(16) glm::vec3 specular;
    alignas(4) float shininess;
  };

  struct Attenuation {
    alignas(4) float constant;
    alignas(4) float linear;
    alignas(4) float quadratic;
  };

  struct PointLight {
    alignas(16) glm::vec3 position;

    alignas(16) glm::vec3 ambient;
    alignas(16) glm::vec3 diffuse;
    alignas(16) glm::vec3 specular;

    alignas(16) Attenuation attenuation;
  };

  alignas(16) glm::mat4 view;
  alignas(16) glm::mat4 projection;

  alignas(16) Material material;

  PointLight pointLights[MAX_LIGHTS];
  alignas(4) u32 pointLightCount;
};

#endif  // SRC_RENDERER_VULKAN_BUFFER_STRUCTURES_GLOBALRENDERUNIFORM_H_
