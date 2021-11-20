#ifndef VULKAN_SRC_GAME_RENDERS_SPACESHIPS_SPACESHIPRENDER_H
#define VULKAN_SRC_GAME_RENDERS_SPACESHIPS_SPACESHIPRENDER_H

#include <game/renders/ActorSpawnController.h>

#include "PlayerController.h"
#include "PlayerSpawnConsumer.h"
#include "game/Camera.h"
#include "game/renders/ParticleController.h"
#include "game/renders/SceneRender.h"
#include "general/windowing/Window.h"

class SpaceshipRender : public SceneRender, public PlayerSpawnConsumer {
 public:
  SpaceshipRender(Camera& camera, ParticleController& particleController,
                  const PlayerController& mainPlayerController);

  std::unique_ptr<PipelineStateFactory> ConfigurePipeline() const override;
  std::unique_ptr<DescriptorConfiguration> ConfigureDescriptors()
      const override;
  void LoadActors(ResourceLoader& resourceLoader,
                  ActorSpawnController& actorSpawnController) override;

  void SpawnPlayer(const PlayerController& controller) override;

 private:
  Camera& camera_;
  ParticleController& particleController_;
  const PlayerController& mainPlayerController_;
  PlayerSpawnConsumer* spawnConsumer_;
};

#endif  // VULKAN_SRC_GAME_RENDERS_SPACESHIPS_SPACESHIPRENDER_H
