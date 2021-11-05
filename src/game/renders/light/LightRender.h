#ifndef VULKAN_SRC_GAME_RENDERS_LIGHT_LIGHTRENDER_H
#define VULKAN_SRC_GAME_RENDERS_LIGHT_LIGHTRENDER_H

#include "game/renders/SceneRender.h"

class LightRender : public SceneRender {
 public:
  std::unique_ptr<PipelineStateFactory> ConfigurePipeline() const override;
  std::unique_ptr<DescriptorConfiguration> ConfigureDescriptors()
      const override;
  std::vector<std::unique_ptr<Actor>> LoadActors(
      const ResourceLoader& resourceLoader) const override;
};

#endif  // VULKAN_SRC_GAME_RENDERS_LIGHT_LIGHTRENDER_H
