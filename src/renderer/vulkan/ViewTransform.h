#ifndef VULKAN_SRC_RENDERER_VULKAN_VIEWTRANSFORM_H_
#define VULKAN_SRC_RENDERER_VULKAN_VIEWTRANSFORM_H_

#include "util/include/glm.h"

struct Material {
  alignas(16) glm::vec3 ambient;
  alignas(16) glm::vec3 diffuse;
  alignas(16) glm::vec3 specular;
  alignas(4) float shininess;
};

struct Light {
  alignas(16) glm::vec3 position;

  alignas(16) glm::vec3 ambient;
  alignas(16) glm::vec3 diffuse;
  alignas(16) glm::vec3 specular;
};

struct PerFrameData {
  alignas(16) glm::mat4 projection;
  alignas(16) glm::mat4 view;
  alignas(16) glm::vec3 cameraPosition;
  alignas(16) glm::vec3 lightingPosition;
  Material material;
  Light light;
};

#endif  // VULKAN_SRC_RENDERER_VULKAN_VIEWTRANSFORM_H_
