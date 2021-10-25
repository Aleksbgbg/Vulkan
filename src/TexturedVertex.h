#ifndef VULKAN_SRC_TEXTUREDVERTEX_H
#define VULKAN_SRC_TEXTUREDVERTEX_H

#include "util/include/glm.h"

struct TexturedVertex {
  bool operator==(const TexturedVertex& other) const {
    return (position == other.position) && (normal == other.normal) &&
           (textureCoordinate == other.textureCoordinate);
  }

  glm::vec3 position;
  glm::vec3 normal;
  glm::vec2 textureCoordinate;
};

namespace std {

template <>
class hash<TexturedVertex> {
 public:
  size_t operator()(const TexturedVertex& vertex) const {
    return std::hash<float>()(vertex.position.x) ^
           std::hash<float>()(vertex.position.y) ^
           std::hash<float>()(vertex.position.z) ^
           std::hash<float>()(vertex.normal.x) ^
           std::hash<float>()(vertex.normal.y) ^
           std::hash<float>()(vertex.normal.z) ^
           std::hash<float>()(vertex.textureCoordinate.x) ^
           std::hash<float>()(vertex.textureCoordinate.y);
  }
};

}  // namespace std

#endif  // VULKAN_SRC_TEXTUREDVERTEX_H
