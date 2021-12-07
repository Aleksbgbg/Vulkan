#ifndef VULKAN_SRC_GAME_BEHAVIOURS_PLAYERMOVEMENT_H_
#define VULKAN_SRC_GAME_BEHAVIOURS_PLAYERMOVEMENT_H_

#include "game/actor/behaviour/Behaviour.h"
#include "game/actor/property/Transform.h"

class PlayerMovement : public Behaviour {
 public:
  PlayerMovement(Transform& transform);

  void UpdateModel(const UpdateContext& context) override;

 private:
  Transform& transform_;
  glm::quat rotation_;
  float velocity_;
};

#endif  // VULKAN_SRC_GAME_BEHAVIOURS_PLAYERMOVEMENT_H_
