#ifndef VULKAN_SRC_APPNETWORK_H
#define VULKAN_SRC_APPNETWORK_H

#include "game/Network.h"
#include "game/model/UpdateContext.h"
#include "game/renders/spaceships/PlayerSpawnConsumer.h"

class AppNetwork : public Network {
 public:
  virtual ~AppNetwork() = default;

  virtual u32 ActorIndexForCurrentApp() const = 0;
  virtual void Update(const UpdateContext& context) = 0;
  virtual void ExecuteSpawns(PlayerSpawnConsumer& spawnConsumer) = 0;
};

#endif  // VULKAN_SRC_APPNETWORK_H
