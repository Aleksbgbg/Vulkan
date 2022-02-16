#include "SceneComposer.h"

#include "game/behaviours/CameraController.h"
#include "game/composition/behaviour_utils.h"
#include "game/ui/UiDrawList.h"
#include "game/ui/parse_xml_tree.h"
#include "game/ui/produce_ui_tree.h"

SceneComposer::SceneComposer(game::Camera& camera, ActorConsumer& actorConsumer,
                             ActorOwner& actorOwner, Renderer& renderer,
                             sys::Window& window, sys::Sound& sound,
                             const FontAtlas& fontAtlas)
    : actorKeyGenerator_(),
      camera_(camera),
      spawnDependencies_({.actorConsumer = &actorConsumer,
                          .actorOwner = &actorOwner,
                          .renderer = &renderer,
                          .window = &window,
                          .sound = &sound,
                          .actorKeyGenerator = &actorKeyGenerator_,
                          .fontAtlas = &fontAtlas}) {}

MeshHandle SceneComposer::LoadMesh(const RenderType renderType,
                                   const MeshLoadParams& meshLoadParams) {
  return spawnDependencies_.renderer->LoadMesh(renderType, meshLoadParams);
}

CompositionBuilder SceneComposer::Actor() {
  return CompositionBuilder::Actor(spawnDependencies_);
}

CompositionBuilder SceneComposer::Camera() {
  return std::move(
      CompositionBuilder::Actor(spawnDependencies_)
          .Attach(BEHAVIOUR(CameraController, camera_,
                            parent->RetrieveProperty<Transform>())));
}

CompositionBuilder SceneComposer::ParticleSystem(
    const ParticleBehaviour behaviour) {
  return CompositionBuilder::ParticleSystem(spawnDependencies_, behaviour);
}

CompositionBuilder SceneComposer::UiElement(
    const std::string_view content, std::unique_ptr<ViewModel> viewModel) {
  return CompositionBuilder::Ui(spawnDependencies_, content,
                                std::move(viewModel));
}
