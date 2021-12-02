#ifndef VULKAN_SRC_GAME_RENDERS_LIGHT_LIGHTRENDER_H_
#define VULKAN_SRC_GAME_RENDERS_LIGHT_LIGHTRENDER_H_

#include <game/renders/ActorSpawnController.h>

#include "game/renders/SceneRender.h"

class LightRender : public SceneRender {
 public:
  std::unique_ptr<PipelineStateFactory> ConfigurePipeline() const override;
  std::unique_ptr<DescriptorConfiguration> ConfigureDescriptors()
      const override;
  void LoadActors(ResourceLoader& resourceLoader,
                  ActorSpawnController& actorSpawnController) override;
};

#endif  // VULKAN_SRC_GAME_RENDERS_LIGHT_LIGHTRENDER_H_
