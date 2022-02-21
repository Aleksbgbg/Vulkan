#ifndef VULKAN_SRC_ENGINE_COMPOSITION_LIGHTSOURCE_H_
#define VULKAN_SRC_ENGINE_COMPOSITION_LIGHTSOURCE_H_

#include "util/include/glm.h"

enum class LightSourceType {
  PointLight = 1,
};

struct LightSource {
  LightSourceType type;

  struct PointLightInfo {
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    struct Attenuation {
      float constant;
      float linear;
      float quadratic;
    };
    Attenuation attenuation;
  };

  union {
    PointLightInfo pointLight;
  } info;
};

#endif  // VULKAN_SRC_ENGINE_COMPOSITION_LIGHTSOURCE_H_
