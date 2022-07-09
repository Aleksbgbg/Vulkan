#include "PositionTextureVertex.h"

#include "core/files/vertex_loader.h"
#include "util/include/glm_hash.h"

DrawVertices<PositionTextureVertex> PositionTextureVertex::LoadVertices(
    const asset::Model model) {
  return LoadDrawVertices<PositionTextureVertex>(
      model, [](const auto& positionVertex, const auto& normalVertex,
                const auto& textureVertex) {
        PositionTextureVertex vertex;
        vertex.position.x = positionVertex.x;
        vertex.position.y = positionVertex.y;
        vertex.position.z = positionVertex.z;
        vertex.textureCoordinate.x = textureVertex.u;
        vertex.textureCoordinate.y = textureVertex.v;
        return vertex;
      });
}

std::unique_ptr<StructuredVertexData> PositionTextureVertex::LoadVertexData(
    const asset::Model model) {
  return std::make_unique<VertexDataOfType<PositionTextureVertex>>(
      PositionTextureVertex::LoadVertices(model));
}

bool PositionTextureVertex::operator==(
    const PositionTextureVertex& other) const {
  return (position == other.position) &&
         (textureCoordinate == other.textureCoordinate);
}

namespace std {

size_t hash<PositionTextureVertex>::operator()(
    const PositionTextureVertex& vertex) const {
  return std::hash<glm::vec3>()(vertex.position) ^
         std::hash<glm::vec2>()(vertex.textureCoordinate);
}

}  // namespace std
