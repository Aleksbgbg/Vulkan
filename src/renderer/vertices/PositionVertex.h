#ifndef VULKAN_SRC_RENDERER_VERTICES_POSITIONVERTEX_H_
#define VULKAN_SRC_RENDERER_VERTICES_POSITIONVERTEX_H_

#include <memory>
#include <string_view>

#include "StructuredVertexData.h"
#include "util/include/glm.h"

struct PositionVertex {
  static std::unique_ptr<StructuredVertexData> LoadVertexData(
      const std::string_view filename);

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

#endif  // VULKAN_SRC_RENDERER_VERTICES_POSITIONVERTEX_H_
