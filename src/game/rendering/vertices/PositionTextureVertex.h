#ifndef VULKAN_SRC_GAME_SKYBOXVERTEX_H
#define VULKAN_SRC_GAME_SKYBOXVERTEX_H

#include <string_view>

#include "general/files/DrawVertices.h"
#include "util/include/glm.h"

struct PositionTextureVertex {
  static DrawVertices<PositionTextureVertex> LoadVertices(
      const std::string_view filename);

  bool operator==(const PositionTextureVertex& other) const;

  glm::vec3 position;
  glm::vec2 textureCoordinate;
};

namespace std {

template <>
class hash<PositionTextureVertex> {
 public:
  size_t operator()(const PositionTextureVertex& vertex) const;
};

}  // namespace std

#endif  // VULKAN_SRC_GAME_SKYBOXVERTEX_H
