#ifndef VULKAN_SRC_GAME_RENDERS_SPACESHIPS_NOMOVEMENTPLAYERCONTROLLER_H
#define VULKAN_SRC_GAME_RENDERS_SPACESHIPS_NOMOVEMENTPLAYERCONTROLLER_H

#include "PlayerController.h"

class NoMovementPlayerController : public PlayerController {
 public:
  bool IsMoving() const override;
  glm::vec3 GetRotation() const override;
  glm::vec3 GetPosition() const override;
};

#endif  // VULKAN_SRC_GAME_RENDERS_SPACESHIPS_NOMOVEMENTPLAYERCONTROLLER_H
