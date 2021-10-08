#ifndef VULKAN_SRC_TEXTUREDVERTEX_H
#define VULKAN_SRC_TEXTUREDVERTEX_H

#include "include_glm.h"

struct TexturedVertex {
  glm::vec3 position;
  glm::vec2 textureCoordinate;
};

#endif  // VULKAN_SRC_TEXTUREDVERTEX_H
