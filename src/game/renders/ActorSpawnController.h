#ifndef VULKAN_SRC_GAME_RENDERS_ACTORSPAWNCONTROLLER_H_
#define VULKAN_SRC_GAME_RENDERS_ACTORSPAWNCONTROLLER_H_

#include <memory>

#include "ActorGenerator.h"
#include "game/Actor.h"
#include "game/rendering/resources/ResourceBinder.h"
#include "game/rendering/resources/ResourceLoader.h"

class ActorSpawnController {
 public:
  virtual ~ActorSpawnController() = default;

  virtual Actor& SpawnActor(const ActorGenerator& actorGenerator) = 0;
  virtual void SpawnActorsImmediately(
      std::vector<std::unique_ptr<Actor>> actors) = 0;
};

#endif  // VULKAN_SRC_GAME_RENDERS_ACTORSPAWNCONTROLLER_H_
