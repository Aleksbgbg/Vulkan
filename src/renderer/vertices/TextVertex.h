#ifndef VULKAN_SRC_RENDERER_VERTICES_TEXTVERTEX_H_
#define VULKAN_SRC_RENDERER_VERTICES_TEXTVERTEX_H_

#include "util/include/glm.h"

struct TextVertex {
  bool operator==(const TextVertex& other) const;

  glm::vec3 position;
  glm::vec2 textureCoordinate;
  glm::vec3 color;
};

namespace std {

template <>
class hash<TextVertex> {
 public:
  size_t operator()(const TextVertex& vertex) const;
};

}  // namespace std

#endif  // VULKAN_SRC_RENDERER_VERTICES_TEXTVERTEX_H_
