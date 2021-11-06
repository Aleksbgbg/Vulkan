#ifndef VULKAN_SRC_GAME_SKYBOXVERTEX_H
#define VULKAN_SRC_GAME_SKYBOXVERTEX_H

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

#include "general/files/DrawVertices.h"
#include "util/include/glm.h"

struct PositionTextureVertex {
  static DrawVertices<PositionTextureVertex> LoadVertices(
      const std::string filename);

  bool operator==(const PositionTextureVertex& other) const;

  glm::vec3 position;
  glm::vec2 textureCoordinate;
};

namespace std {

template <>
class hash<PositionTextureVertex> {
 public:
  size_t operator()(const PositionTextureVertex& vertex) const {
    return std::hash<glm::vec3>()(vertex.position) ^
           std::hash<glm::vec2>()(vertex.textureCoordinate);
  }
};

}  // namespace std

#endif  // VULKAN_SRC_GAME_SKYBOXVERTEX_H
