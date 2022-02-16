#ifndef VULKAN_SRC_GAME_COMPOSITION_SPAWNDEPENDENCIES_H_
#define VULKAN_SRC_GAME_COMPOSITION_SPAWNDEPENDENCIES_H_

#include "ActorConsumer.h"
#include "game/Renderer.h"
#include "game/actor/ActorOwner.h"
#include "general/text/FontAtlas.h"
#include "system/sound/Sound.h"
#include "system/windowing/Window.h"

struct SpawnDependencies {
  ActorConsumer* actorConsumer;
  ActorOwner* actorOwner;
  Renderer* renderer;
  sys::Window* window;
  sys::Sound* sound;
  ActorKeyGenerator* actorKeyGenerator;
  const FontAtlas* fontAtlas;
};

#endif  // VULKAN_SRC_GAME_COMPOSITION_SPAWNDEPENDENCIES_H_
