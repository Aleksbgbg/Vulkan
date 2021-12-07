#ifndef VULKAN_SRC_GAME_ACTOR_BEHAVIOUR_BEHAVIOUR_H_
#define VULKAN_SRC_GAME_ACTOR_BEHAVIOUR_BEHAVIOUR_H_

#include "game/UpdateContext.h"

class Behaviour {
 public:
  virtual ~Behaviour() = default;

  virtual void OnSpawn() {}
  virtual void UpdateModel(const UpdateContext& context) {}
};

#endif  // VULKAN_SRC_GAME_ACTOR_BEHAVIOUR_BEHAVIOUR_H_
