#ifndef VULKAN_SRC_GAME_RENDERS_SPACESHIPS_NOMOVEMENTPLAYERCONTROLLER_H
#define VULKAN_SRC_GAME_RENDERS_SPACESHIPS_NOMOVEMENTPLAYERCONTROLLER_H

#include "PlayerController.h"

class NoMovementPlayerController : public PlayerController {
 public:
  glm::vec3 GetVelocity() const override;
  glm::vec3 GetRotation() const override;
};

#endif  // VULKAN_SRC_GAME_RENDERS_SPACESHIPS_NOMOVEMENTPLAYERCONTROLLER_H
