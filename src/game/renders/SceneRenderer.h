#ifndef VULKAN_SRC_GAME_RENDERS_SCENERENDERER_H
#define VULKAN_SRC_GAME_RENDERS_SCENERENDERER_H

#include "ActorDescriptorAllocator.h"
#include "ActorDescriptorBinder.h"
#include "ActorSpawnController.h"
#include "SceneDescriptorBinder.h"
#include "SceneRender.h"
#include "game/RenderPipeline.h"
#include "game/model/UpdateContext.h"
#include "game/rendering/meshes/MeshRenderer.h"

class SceneRenderer : public ActorSpawnController {
 public:
  class ResourceBinderFactory {
   public:
    virtual ~ResourceBinderFactory() = default;

    virtual std::unique_ptr<ResourceBinder> CreateResourceBinder(
        const DescriptorSet& descriptorSet) const = 0;
  };

  SceneRenderer(ResourceLoader& resourceLoader,
                std::unique_ptr<ResourceBinderFactory> resourceBinderFactory,
                std::unique_ptr<ActorDescriptorAllocator> descriptorAllocator,
                std::unique_ptr<SceneRender> render,
                RenderPipeline renderPipeline,
                std::unique_ptr<ActorDescriptorBinder> descriptorBinder);

  Actor& SpawnActor(const ActorGenerator& actorGenerator) override;
  void SpawnActorsImmediately(
      std::vector<std::unique_ptr<Actor>> actors) override;

  void UpdateModel(const UpdateContext& updateContext);
  void Render(const CommandBuffer& commandBuffer,
              const SceneDescriptorBinder& sceneDescriptorBinder,
              const wnd::Window& window) const;

 private:
  ResourceLoader& resourceLoader_;
  std::unique_ptr<ResourceBinderFactory> resourceBinderFactory_;
  std::unique_ptr<SceneRender> render_;
  RenderPipeline renderPipeline_;
  std::unique_ptr<ActorDescriptorAllocator> descriptorAllocator_;
  std::unique_ptr<ActorDescriptorBinder> descriptorBinder_;
  std::vector<std::unique_ptr<Actor>> actors_;
};

#endif  // VULKAN_SRC_GAME_RENDERS_SCENERENDERER_H
