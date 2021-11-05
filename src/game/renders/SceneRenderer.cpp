#include "SceneRenderer.h"

SceneRenderer::SceneRenderer(
    RenderPipeline renderPipeline,
    std::unique_ptr<ActorDescriptorBinder> descriptorBinder,
    std::vector<std::unique_ptr<Actor>> actors)
    : renderPipeline(std::move(renderPipeline)),
      descriptorBinder(std::move(descriptorBinder)),
      actors(std::move(actors)) {}

void SceneRenderer::UpdateModel(const UpdateContext& updateContext) {
  for (const std::unique_ptr<Actor>& actor : actors) {
    actor->UpdateModel(updateContext);
  }
}

void SceneRenderer::Render(const CommandBuffer& commandBuffer,
                           const SceneDescriptorBinder& sceneDescriptorBinder,
                           const Window& window) const {
  renderPipeline.Bind(commandBuffer, window);
  sceneDescriptorBinder.BindActorDescriptorSet(commandBuffer,
                                               renderPipeline.GetLayout());

  const MeshRenderer meshRenderer(commandBuffer, renderPipeline.GetLayout());

  for (u32 actorIndex = 0; actorIndex < actors.size(); ++actorIndex) {
    descriptorBinder->BindActorDescriptorSet(renderPipeline.GetLayout(),
                                             commandBuffer, actorIndex);
    actors[actorIndex]->Render(meshRenderer);
  }
}
