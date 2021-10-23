#ifndef VULKAN_SRC_GAME_ACTOR_H
#define VULKAN_SRC_GAME_ACTOR_H

#include "game/model/Updatable.h"
#include "game/rendering/Renderable.h"

class Actor : public Updatable, public Renderable {
 public:
  virtual ~Actor() = default;
};

#endif  // VULKAN_SRC_GAME_ACTOR_H
