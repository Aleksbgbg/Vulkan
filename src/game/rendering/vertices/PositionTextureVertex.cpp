#include "PositionTextureVertex.h"

#include "general/files/vertex_loader.h"

DrawVertices<PositionTextureVertex> PositionTextureVertex::LoadVertices(
    const std::string filename) {
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
