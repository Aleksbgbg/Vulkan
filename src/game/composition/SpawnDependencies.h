#ifndef VULKAN_SRC_GAME_COMPOSITION_SPAWNDEPENDENCIES_H_
#define VULKAN_SRC_GAME_COMPOSITION_SPAWNDEPENDENCIES_H_

#include "ActorConsumer.h"
#include "game/Renderer.h"
#include "game/actor/ActorOwner.h"
#include "system/sound/Sound.h"

struct SpawnDependencies {
  ActorConsumer* actorConsumer;
  ActorOwner* actorOwner;
  Renderer* renderer;
  sys::Sound* sound;
};

#endif  // VULKAN_SRC_GAME_COMPOSITION_SPAWNDEPENDENCIES_H_
