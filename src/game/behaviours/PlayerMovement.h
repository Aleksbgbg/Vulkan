#ifndef VULKAN_SRC_GAME_BEHAVIOURS_PLAYERMOVEMENT_H_
#define VULKAN_SRC_GAME_BEHAVIOURS_PLAYERMOVEMENT_H_

#include "engine/behaviour/Behaviour.h"
#include "engine/property/type/Transform.h"

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
