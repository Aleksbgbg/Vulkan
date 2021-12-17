#ifndef VULKAN_SRC_GAME_COMPOSITION_ACTORTOSPAWN_H_
#define VULKAN_SRC_GAME_COMPOSITION_ACTORTOSPAWN_H_

#include <memory>

#include "game/actor/Actor.h"
#include "game/actor/ActorKey.h"

struct ActorToSpawn {
  ActorKey parent;
  ActorKey key;
  std::unique_ptr<game::Actor> actor;
};

#endif  // VULKAN_SRC_GAME_COMPOSITION_ACTORTOSPAWN_H_
