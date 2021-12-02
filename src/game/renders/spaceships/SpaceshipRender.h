#ifndef VULKAN_SRC_GAME_RENDERS_SPACESHIPS_SPACESHIPRENDER_H_
#define VULKAN_SRC_GAME_RENDERS_SPACESHIPS_SPACESHIPRENDER_H_

#include "game/Camera.h"
#include "game/renders/ActorSpawnController.h"
#include "game/renders/ParticleController.h"
#include "game/renders/SceneRender.h"

class SpaceshipRender : public SceneRender {
 public:
  SpaceshipRender(Camera& camera, ParticleController& particleController);

  std::unique_ptr<PipelineStateFactory> ConfigurePipeline() const override;
  std::unique_ptr<DescriptorConfiguration> ConfigureDescriptors()
      const override;
  void LoadActors(ResourceLoader& resourceLoader,
                  ActorSpawnController& actorSpawnController) override;

 private:
  Camera& camera;
  ParticleController& particleController;
};

#endif  // VULKAN_SRC_GAME_RENDERS_SPACESHIPS_SPACESHIPRENDER_H_
