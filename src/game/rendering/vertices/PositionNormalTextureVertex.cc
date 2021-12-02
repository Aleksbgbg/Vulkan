#include "PositionNormalTextureVertex.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

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
