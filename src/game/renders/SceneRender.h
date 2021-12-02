#ifndef VULKAN_SRC_GAME_RENDERS_SCENERENDER_H_
#define VULKAN_SRC_GAME_RENDERS_SCENERENDER_H_

#include <memory>
#include <vector>

#include "DescriptorConfiguration.h"
#include "game/Actor.h"
#include "game/RenderPipeline.h"
#include "game/rendering/resources/ResourceLoader.h"
#include "vulkan/DescriptorSetLayout.h"

class SceneRender {
 public:
  virtual ~SceneRender() = default;

  virtual std::unique_ptr<PipelineStateFactory> ConfigurePipeline() const = 0;
  virtual std::unique_ptr<DescriptorConfiguration> ConfigureDescriptors()
      const = 0;
  virtual void LoadActors(ResourceLoader& resourceLoader,
                          ActorSpawnController& actorSpawnController) = 0;
};

#endif  // VULKAN_SRC_GAME_RENDERS_SCENERENDER_H_
