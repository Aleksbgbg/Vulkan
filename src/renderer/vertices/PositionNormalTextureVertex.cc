#include "PositionNormalTextureVertex.h"

#include "core/files/vertex_loader.h"
#include "util/include/glm_hash.h"

std::unique_ptr<StructuredVertexData>
PositionNormalTextureVertex::LoadVertexData(const asset::Model model) {
  return std::make_unique<VertexDataOfType<PositionNormalTextureVertex>>(
      LoadDrawVertices<PositionNormalTextureVertex>(
          model, [](const auto& positionVertex, const auto& normalVertex,
                    const auto& textureVertex) {
            PositionNormalTextureVertex vertex;
            vertex.position.x = positionVertex.x;
            vertex.position.y = positionVertex.y;
            vertex.position.z = positionVertex.z;
            vertex.normal.x = normalVertex.x;
            vertex.normal.y = normalVertex.y;
            vertex.normal.z = normalVertex.z;
            vertex.textureCoordinate.x = textureVertex.u;
            vertex.textureCoordinate.y = textureVertex.v;
            return vertex;
          }));
}

bool PositionNormalTextureVertex::operator==(
    const PositionNormalTextureVertex& other) const {
  return (position == other.position) && (normal == other.normal) &&
         (textureCoordinate == other.textureCoordinate);
}

namespace std {

size_t hash<PositionNormalTextureVertex>::operator()(
    const PositionNormalTextureVertex& vertex) const {
  return std::hash<glm::vec3>()(vertex.position) ^
         std::hash<glm::vec3>()(vertex.normal) ^
         std::hash<glm::vec2>()(vertex.textureCoordinate);
}

}  // namespace std
