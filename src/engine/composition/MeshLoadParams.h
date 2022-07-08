#ifndef SRC_ENGINE_COMPOSITION_MESHLOADPARAMS_H_
#define SRC_ENGINE_COMPOSITION_MESHLOADPARAMS_H_

#include <optional>
#include <string_view>

#include "util/include/glm.h"

struct MeshLoadParams {
  std::string_view model;
  std::optional<std::string_view> texture;
  std::optional<std::string_view> emissive;
  std::optional<glm::mat4> meshTransform;
};

#endif  // SRC_ENGINE_COMPOSITION_MESHLOADPARAMS_H_
