#ifndef VULKAN_SRC_TEXTUREDVERTEX_H
#define VULKAN_SRC_TEXTUREDVERTEX_H

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
  size_t operator()(const PositionNormalTextureVertex& vertex) const;
};

}  // namespace std

#endif  // VULKAN_SRC_TEXTUREDVERTEX_H
