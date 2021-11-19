#ifndef VULKAN_SRC_GAME_RENDERS_ACTORGENERATOR_H
#define VULKAN_SRC_GAME_RENDERS_ACTORGENERATOR_H

#include <memory>

#include "game/Actor.h"
#include "game/rendering/resources/ResourceBinder.h"
#include "game/rendering/resources/ResourceLoader.h"

class ActorGenerator {
 public:
  virtual ~ActorGenerator() = default;

  virtual std::unique_ptr<Actor> CreateActor(
      ResourceLoader& resourceLoader, ResourceBinder& resourceBinder) const = 0;
};

#endif  // VULKAN_SRC_GAME_RENDERS_ACTORGENERATOR_H
