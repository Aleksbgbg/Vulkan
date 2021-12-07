#ifndef VULKAN_SRC_RENDERER_VERTICES_POSITIONNORMALTEXTUREVERTEX_H_
#define VULKAN_SRC_RENDERER_VERTICES_POSITIONNORMALTEXTUREVERTEX_H_

#include <memory>
#include <string_view>

#include "StructuredVertexData.h"
#include "util/include/glm.h"

struct PositionNormalTextureVertex {
  static std::unique_ptr<StructuredVertexData> LoadVertexData(
      const std::string_view filename);

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

#endif  // VULKAN_SRC_RENDERER_VERTICES_POSITIONNORMALTEXTUREVERTEX_H_
