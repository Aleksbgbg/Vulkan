#ifndef VULKAN_SRC_GAME_ACTOR_UPDATEABLE_H_
#define VULKAN_SRC_GAME_ACTOR_UPDATEABLE_H_

#include "game/UpdateContext.h"

class Updateable {
 public:
  virtual ~Updateable() = default;

  virtual void UpdateModel(const UpdateContext& context) = 0;
};

#endif  // VULKAN_SRC_GAME_ACTOR_UPDATEABLE_H_
