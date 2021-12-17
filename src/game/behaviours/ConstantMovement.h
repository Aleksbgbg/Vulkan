#ifndef VULKAN_SRC_GAME_BEHAVIOURS_CONSTANTMOVEMENT_H_
#define VULKAN_SRC_GAME_BEHAVIOURS_CONSTANTMOVEMENT_H_

#include "game/actor/behaviour/Behaviour.h"
#include "game/actor/property/Transform.h"

class ConstantMovement : public Behaviour {
 public:
  ConstantMovement(Transform& transform, const glm::vec3 velocity);

  void UpdateModel(const UpdateContext& context) override;

 private:
  Transform& transform_;
  glm::vec3 velocity_;
};

#endif  // VULKAN_SRC_GAME_BEHAVIOURS_CONSTANTMOVEMENT_H_
