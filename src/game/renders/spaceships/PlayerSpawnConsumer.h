#ifndef VULKAN_SRC_GAME_RENDERS_SPACESHIPS_PLAYERSPAWNCONSUMER_H
#define VULKAN_SRC_GAME_RENDERS_SPACESHIPS_PLAYERSPAWNCONSUMER_H

#include <memory>

#include "PlayerController.h"

class PlayerSpawnConsumer {
 public:
  virtual ~PlayerSpawnConsumer() = default;

  virtual void SpawnPlayer(const PlayerController &controller) = 0;
};

#endif  // VULKAN_SRC_GAME_RENDERS_SPACESHIPS_PLAYERSPAWNCONSUMER_H
