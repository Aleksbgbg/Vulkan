#ifndef VULKAN_SRC_GAME_RENDERS_SPACESHIPS_SPACESHIPRENDER_H
#define VULKAN_SRC_GAME_RENDERS_SPACESHIPS_SPACESHIPRENDER_H

#include <game/renders/ActorSpawnController.h>

#include "game/Camera.h"
#include "game/renders/ParticleController.h"
#include "game/renders/SceneRender.h"
#include "general/windowing/Window.h"

class SpaceshipRender : public SceneRender {
 public:
  SpaceshipRender(Camera& camera, const wnd::Window& window,
                  ParticleController& particleController);

  std::unique_ptr<PipelineStateFactory> ConfigurePipeline() const override;
  std::unique_ptr<DescriptorConfiguration> ConfigureDescriptors()
      const override;
  void LoadActors(ResourceLoader& resourceLoader,
                  ActorSpawnController& actorSpawnController) override;

 private:
  Camera& camera;
  const wnd::Window& window;
  ParticleController& particleController;
};

#endif  // VULKAN_SRC_GAME_RENDERS_SPACESHIPS_SPACESHIPRENDER_H
