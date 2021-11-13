#ifndef VULKAN_SRC_GAME_RENDERS_SCENERENDER_H
#define VULKAN_SRC_GAME_RENDERS_SCENERENDER_H

#include <memory>
#include <vector>

#include "DescriptorConfiguration.h"
#include "VulkanContext.h"
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
  virtual std::vector<std::unique_ptr<Actor>> LoadActors(
      const ResourceLoader& resourceLoader) = 0;
};

#endif  // VULKAN_SRC_GAME_RENDERS_SCENERENDER_H
