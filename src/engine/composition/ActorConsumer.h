#ifndef VULKAN_SRC_ENGINE_COMPOSITION_ACTORCONSUMER_H_
#define VULKAN_SRC_ENGINE_COMPOSITION_ACTORCONSUMER_H_

#include "ActorToSpawn.h"

class ActorConsumer {
 public:
  virtual ~ActorConsumer() = default;

  virtual void Consume(ActorToSpawn actor) = 0;
};

#endif  // VULKAN_SRC_ENGINE_COMPOSITION_ACTORCONSUMER_H_
