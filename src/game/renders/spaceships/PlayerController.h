#ifndef VULKAN_SRC_GAME_RENDERS_SPACESHIPS_PLAYERCONTROLLER_H_
#define VULKAN_SRC_GAME_RENDERS_SPACESHIPS_PLAYERCONTROLLER_H_

#include "util/include/glm.h"

class PlayerController {
 public:
  virtual ~PlayerController() = default;

  virtual glm::vec3 GetVelocity() const = 0;
  virtual glm::vec3 GetRotation() const = 0;
};

#endif  // VULKAN_SRC_GAME_RENDERS_SPACESHIPS_PLAYERCONTROLLER_H_
