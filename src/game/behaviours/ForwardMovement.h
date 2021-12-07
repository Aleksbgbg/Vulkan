#ifndef VULKAN_SRC_GAME_BEHAVIOURS_FORWARDMOVEMENT_H_
#define VULKAN_SRC_GAME_BEHAVIOURS_FORWARDMOVEMENT_H_

#include "game/actor/behaviour/Behaviour.h"
#include "game/actor/property/Transform.h"

class ForwardMovement : public Behaviour {
 public:
  ForwardMovement(Transform& transform);

  void UpdateModel(const UpdateContext& context) override;

 private:
  Transform& transform_;
};

#endif  // VULKAN_SRC_GAME_BEHAVIOURS_FORWARDMOVEMENT_H_
