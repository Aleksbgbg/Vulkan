#include "PositionColorVertex.h"

#include "util/include/glm_hash.h"

bool PositionColorVertex::operator==(const PositionColorVertex& other) const {
  return (position == other.position) && (color == other.color);
}

size_t std::hash<PositionColorVertex>::operator()(
    const PositionColorVertex& vertex) const {
  return std::hash<glm::vec3>()(vertex.position) ^
         std::hash<glm::vec3>()(vertex.color);
}
