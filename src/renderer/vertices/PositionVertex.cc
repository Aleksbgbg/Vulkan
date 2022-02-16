#include "PositionVertex.h"

#include "general/files/vertex_loader.h"
#include "util/include/glm_hash.h"

std::unique_ptr<StructuredVertexData> PositionVertex::LoadVertexData(
    const std::string_view filename) {
  return std::make_unique<VertexDataOfType<PositionVertex>>(
      LoadDrawVertices<PositionVertex>(
          filename, [](const auto& positionVertex, const auto& normalVertex,
                       const auto& textureVertex) {
            PositionVertex vertex;
            vertex.position.x = positionVertex.x;
            vertex.position.y = positionVertex.y;
            vertex.position.z = positionVertex.z;
            return vertex;
          }));
}

bool PositionVertex::operator==(const PositionVertex& other) const {
  return position == other.position;
}

namespace std {

size_t hash<PositionVertex>::operator()(const PositionVertex& vertex) const {
  return std::hash<glm::vec3>()(vertex.position);
}

}  // namespace std
