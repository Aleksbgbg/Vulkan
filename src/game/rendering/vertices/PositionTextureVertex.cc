#include "PositionTextureVertex.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

#include "general/files/vertex_loader.h"

DrawVertices<PositionTextureVertex> PositionTextureVertex::LoadVertices(
    const std::string_view filename) {
  return LoadUniqueDrawVertices<PositionTextureVertex>(
      filename, [](const auto& positionVertex, const auto& normalVertex,
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
