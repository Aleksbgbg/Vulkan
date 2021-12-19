#ifndef VULKAN_SRC_RENDERER_VULKAN_VIEWTRANSFORM_H_
#define VULKAN_SRC_RENDERER_VULKAN_VIEWTRANSFORM_H_

#include "util/include/glm.h"
#include "util/types.h"

inline constexpr u32 MAX_LIGHTS = 1024;

struct FrameUniform {
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

#endif  // VULKAN_SRC_RENDERER_VULKAN_VIEWTRANSFORM_H_
