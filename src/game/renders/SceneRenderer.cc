#include "SceneRenderer.h"

SceneRenderer::SceneRenderer(
    ResourceLoader& resourceLoader,
    std::unique_ptr<ResourceBinderFactory> resourceBinderFactory,
    std::unique_ptr<ActorDescriptorAllocator> descriptorAllocator,
    std::unique_ptr<SceneRender> render, RenderPipeline renderPipeline,
    std::unique_ptr<ActorDescriptorBinder> descriptorBinder)
    : resourceLoader_(resourceLoader),
      resourceBinderFactory_(std::move(resourceBinderFactory)),
      descriptorAllocator_(std::move(descriptorAllocator)),
      render_(std::move(render)),
      renderPipeline_(std::move(renderPipeline)),
      descriptorBinder_(std::move(descriptorBinder)),
      actors_() {
  render_->LoadActors(resourceLoader, *this);
}

Actor& SceneRenderer::SpawnActor(const ActorGenerator& actorGenerator) {
  DescriptorSet descriptorSet = descriptorAllocator_->AllocateDescriptor();
  std::unique_ptr<ResourceBinder> resourceBinder =
      resourceBinderFactory_->CreateResourceBinder(descriptorSet);

  std::unique_ptr<Actor> actor =
      actorGenerator.CreateActor(resourceLoader_, *resourceBinder);
  Actor& actorRef = *actor;

  descriptorBinder_->AddDescriptor(std::move(descriptorSet));
  actors_.push_back(std::move(actor));

  return actorRef;
}

void SceneRenderer::SpawnActorsImmediately(
    std::vector<std::unique_ptr<Actor>> actors) {
  for (std::unique_ptr<Actor>& actor : actors) {
    DescriptorSet descriptorSet = descriptorAllocator_->AllocateDescriptor();
    std::unique_ptr<ResourceBinder> resourceBinder =
        resourceBinderFactory_->CreateResourceBinder(descriptorSet);

    actor->GetMesh().BindTexture(*resourceBinder);
    actor->BindBuffers(*resourceBinder);

    descriptorBinder_->AddDescriptor(std::move(descriptorSet));
    actors_.push_back(std::move(actor));
  }
}

void SceneRenderer::UpdateModel(const UpdateContext& updateContext) {
  for (const std::unique_ptr<Actor>& actor : actors_) {
    actor->UpdateModel(updateContext);
  }
}

void SceneRenderer::Render(const CommandBuffer& commandBuffer,
                           const SceneDescriptorBinder& sceneDescriptorBinder,
                           const sys::Window& window) const {
  renderPipeline_.Bind(commandBuffer, window);
  sceneDescriptorBinder.BindActorDescriptorSet(commandBuffer,
                                               renderPipeline_.GetLayout());

  const MeshRenderer meshRenderer(commandBuffer, renderPipeline_.GetLayout());

  for (u32 actorIndex = 0; actorIndex < actors_.size(); ++actorIndex) {
    descriptorBinder_->BindActorDescriptorSet(renderPipeline_.GetLayout(),
                                              commandBuffer, actorIndex);
    actors_[actorIndex]->Render(meshRenderer);
  }
}
