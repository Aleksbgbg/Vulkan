#ifndef VULKAN_SRC_RENDERER_VERTICES_POSITIONCOLORVERTEX_H_
#define VULKAN_SRC_RENDERER_VERTICES_POSITIONCOLORVERTEX_H_

#include "util/include/glm.h"

struct PositionColorVertex {
  bool operator==(const PositionColorVertex& other) const;

  glm::vec3 position;
  glm::vec4 color;
};

namespace std {

template <>
class hash<PositionColorVertex> {
 public:
  size_t operator()(const PositionColorVertex& vertex) const;
};

}  // namespace std

#endif  // VULKAN_SRC_RENDERER_VERTICES_POSITIONCOLORVERTEX_H_
