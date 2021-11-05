#ifndef VULKAN_SRC_GAME_RENDERS_SCENERENDERER_H
#define VULKAN_SRC_GAME_RENDERS_SCENERENDERER_H

#include "ActorDescriptorBinder.h"
#include "SceneDescriptorBinder.h"
#include "SceneRender.h"
#include "VulkanContext.h"
#include "game/RenderPipeline.h"
#include "game/model/UpdateContext.h"
#include "game/rendering/resources/MeshRenderer.h"

class SceneRenderer {
 public:
  SceneRenderer() = default;
  SceneRenderer(RenderPipeline renderPipeline,
                std::unique_ptr<ActorDescriptorBinder> descriptorBinder,
                std::vector<std::unique_ptr<Actor>> actors);

  void UpdateModel(const UpdateContext& updateContext);
  void Render(const CommandBuffer& commandBuffer,
              const SceneDescriptorBinder& sceneDescriptorBinder,
              const Window& window) const;

 private:
  RenderPipeline renderPipeline;
  std::unique_ptr<ActorDescriptorBinder> descriptorBinder;
  std::vector<std::unique_ptr<Actor>> actors;
};

#endif  // VULKAN_SRC_GAME_RENDERS_SCENERENDERER_H
