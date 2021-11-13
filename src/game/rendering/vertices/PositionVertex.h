#ifndef VULKAN_SRC_GAME_RENDERING_VERTICES_POSITIONVERTEX_H
#define VULKAN_SRC_GAME_RENDERING_VERTICES_POSITIONVERTEX_H

#include "util/include/glm.h"

struct PositionVertex {
  bool operator==(const PositionVertex& other) const;

  glm::vec3 position;
};

namespace std {

template <>
class hash<PositionVertex> {
 public:
  size_t operator()(const PositionVertex& vertex) const;
};

}  // namespace std

#endif  // VULKAN_SRC_GAME_RENDERING_VERTICES_POSITIONVERTEX_H
