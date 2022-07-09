#ifndef SRC_ENGINE_COMPOSITION_MESHLOADPARAMS_H_
#define SRC_ENGINE_COMPOSITION_MESHLOADPARAMS_H_

#include <optional>

#include "asset.h"
#include "util/include/glm.h"

struct MeshLoadParams {
  asset::Model model;
  std::optional<asset::Texture> texture;
  std::optional<asset::Texture> emissive;
  std::optional<glm::mat4> meshTransform;
};

#endif  // SRC_ENGINE_COMPOSITION_MESHLOADPARAMS_H_
