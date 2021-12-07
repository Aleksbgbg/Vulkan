#ifndef VULKAN_SRC_GAME_COMPOSITION_ACTORCONSUMER_H_
#define VULKAN_SRC_GAME_COMPOSITION_ACTORCONSUMER_H_

#include <memory>

#include "game/actor/Actor.h"
#include "general/operations/Consumer.h"

typedef Consumer<std::unique_ptr<game::Actor>> ActorConsumer;

#endif  // VULKAN_SRC_GAME_COMPOSITION_ACTORCONSUMER_H_
