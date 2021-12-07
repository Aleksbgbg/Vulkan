#ifndef VULKAN_SRC_GAME_TRANSFORMABLE_H_
#define VULKAN_SRC_GAME_TRANSFORMABLE_H_

#include "util/include/glm.h"

class Transformable {
 public:
  virtual ~Transformable() = default;

  virtual glm::mat4 GetTransform() const = 0;
};

#endif  // VULKAN_SRC_GAME_TRANSFORMABLE_H_
