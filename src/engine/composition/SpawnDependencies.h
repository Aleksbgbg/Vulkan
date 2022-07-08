#ifndef SRC_ENGINE_COMPOSITION_SPAWNDEPENDENCIES_H_
#define SRC_ENGINE_COMPOSITION_SPAWNDEPENDENCIES_H_

#include "ActorConsumer.h"
#include "Renderer.h"
#include "core/text/FontAtlas.h"
#include "engine/actor/ActorOwner.h"
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

#endif  // SRC_ENGINE_COMPOSITION_SPAWNDEPENDENCIES_H_
