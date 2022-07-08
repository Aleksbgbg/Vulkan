#ifndef SRC_RENDERER_VERTICES_POSITIONTEXTUREVERTEX_H_
#define SRC_RENDERER_VERTICES_POSITIONTEXTUREVERTEX_H_

#include <memory>
#include <string_view>

#include "StructuredVertexData.h"
#include "core/files/DrawVertices.h"
#include "util/include/glm.h"

struct PositionTextureVertex {
  static std::unique_ptr<StructuredVertexData> LoadVertexData(
      const std::string_view filename);
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

#endif  // SRC_RENDERER_VERTICES_POSITIONTEXTUREVERTEX_H_
