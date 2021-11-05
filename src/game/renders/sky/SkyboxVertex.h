#ifndef VULKAN_SRC_GAME_SKYBOXVERTEX_H
#define VULKAN_SRC_GAME_SKYBOXVERTEX_H

#include "util/include/glm.h"

struct SkyboxVertex {
  glm::vec3 position;
  glm::vec2 textureCoordinate;
};

#endif  // VULKAN_SRC_GAME_SKYBOXVERTEX_H
