#ifndef VULKAN_SRC_GAME_BEHAVIOURS_SUNMOVEMENT_H_
#define VULKAN_SRC_GAME_BEHAVIOURS_SUNMOVEMENT_H_

#include "game/actor/behaviour/Behaviour.h"
#include "game/actor/property/Transform.h"

class SunMovement : public Behaviour {
 public:
  SunMovement(Transform& transform);

  void OnSpawn() override;
  void UpdateModel(const UpdateContext& context) override;

 private:
  Transform& transform_;
  float spin_;
};

#endif  // VULKAN_SRC_GAME_BEHAVIOURS_SUNMOVEMENT_H_
