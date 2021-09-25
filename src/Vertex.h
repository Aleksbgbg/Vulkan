#ifndef VULKAN_SRC_VERTEX_H
#define VULKAN_SRC_VERTEX_H

#include <glm/glm.hpp>

struct Vertex {
  glm::vec3 pos;
  glm::vec3 color;
  glm::vec2 texCoord;
};

#endif // VULKAN_SRC_VERTEX_H
