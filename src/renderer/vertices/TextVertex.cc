#include "TextVertex.h"

#include "util/include/glm_hash.h"

bool TextVertex::operator==(const TextVertex& other) const {
  return (position == other.position) &&
         (textureCoordinate == other.textureCoordinate) &&
         (color == other.color);
}

size_t std::hash<TextVertex>::operator()(const TextVertex& vertex) const {
  return std::hash<glm::vec3>()(vertex.position) ^
         std::hash<glm::vec2>()(vertex.textureCoordinate) ^
         std::hash<glm::vec3>()(vertex.color);
}
