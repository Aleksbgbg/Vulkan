#include "PositionNormalTextureVertex.h"

bool PositionNormalTextureVertex::operator==(
    const PositionNormalTextureVertex& other) const {
  return (position == other.position) && (normal == other.normal) &&
         (textureCoordinate == other.textureCoordinate);
}
