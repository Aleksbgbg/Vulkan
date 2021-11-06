#ifndef VULKAN_SRC_TEXTUREDVERTEX_H
#define VULKAN_SRC_TEXTUREDVERTEX_H

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

#include "util/include/glm.h"

struct PositionNormalTextureVertex {
  bool operator==(const PositionNormalTextureVertex& other) const;

  glm::vec3 position;
  glm::vec3 normal;
  glm::vec2 textureCoordinate;
};

namespace std {

template <>
class hash<PositionNormalTextureVertex> {
 public:
  size_t operator()(const PositionNormalTextureVertex& vertex) const {
    return std::hash<glm::vec3>()(vertex.position) ^
           std::hash<glm::vec3>()(vertex.normal) ^
           std::hash<glm::vec2>()(vertex.textureCoordinate);
  }
};

}  // namespace std

#endif  // VULKAN_SRC_TEXTUREDVERTEX_H
