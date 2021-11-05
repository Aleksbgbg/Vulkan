#ifndef VULKAN_SRC_GAME_RENDERS_SKY_SKYBOXRENDER_H
#define VULKAN_SRC_GAME_RENDERS_SKY_SKYBOXRENDER_H

#include "game/renders/SceneRender.h"

class SkyboxRender : public SceneRender {
 public:
  std::unique_ptr<PipelineStateFactory> ConfigurePipeline() const override;
  std::unique_ptr<DescriptorConfiguration> ConfigureDescriptors()
      const override;
  std::vector<std::unique_ptr<Actor>> LoadActors(
      const ResourceLoader& resourceLoader) const override;
};

#endif  // VULKAN_SRC_GAME_RENDERS_SKY_SKYBOXRENDER_H
