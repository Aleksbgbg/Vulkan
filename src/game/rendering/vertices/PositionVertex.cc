#include "PositionVertex.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

bool PositionVertex::operator==(const PositionVertex& other) const {
  return position == other.position;
}

namespace std {

size_t hash<PositionVertex>::operator()(const PositionVertex& vertex) const {
  return std::hash<glm::vec3>()(vertex.position);
}

}  // namespace std
