#ifndef VULKAN_SRC_TEXTUREDVERTEX_H
#define VULKAN_SRC_TEXTUREDVERTEX_H

#include "util/include/glm.h"

struct TexturedVertex {
  bool operator==(const TexturedVertex& other) const {
    return (position == other.position) &&
           (textureCoordinate == other.textureCoordinate);
  }

  glm::vec3 position;
  glm::vec2 textureCoordinate;
};

namespace std {

template <>
class hash<TexturedVertex> {
 public:
  size_t operator()(const TexturedVertex& vertex) const {
    return std::hash<u32>()(vertex.position.x) ^
           std::hash<u32>()(vertex.position.y) ^
           std::hash<u32>()(vertex.position.z) ^
           std::hash<u32>()(vertex.textureCoordinate.x) ^
           std::hash<u32>()(vertex.textureCoordinate.y);
  }
};

}  // namespace std

#endif  // VULKAN_SRC_TEXTUREDVERTEX_H
