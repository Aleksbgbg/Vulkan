#ifndef SRC_ENGINE_COMPOSITION_SCENECOMPOSER_H_
#define SRC_ENGINE_COMPOSITION_SCENECOMPOSER_H_

#include "ActorConsumer.h"
#include "CompositionBuilder.h"
#include "MeshHandle.h"
#include "MeshLoadParams.h"
#include "ParticleBehaviour.h"
#include "Renderer.h"
#include "core/text/FontAtlas.h"
#include "game/Camera.h"

class SceneComposer {
 public:
  SceneComposer(game::Camera& camera, ActorConsumer& actorConsumer,
                ActorOwner& actorOwner, Renderer& renderer, sys::Window& window,
                sys::Sound& sound, const FontAtlas& fontAtlas);

  MeshHandle LoadMesh(const RenderType renderType,
                      const MeshLoadParams& meshLoadParams);

  CompositionBuilder Actor();
  CompositionBuilder Camera();
  CompositionBuilder ParticleSystem(const ParticleBehaviour behaviour);
  CompositionBuilder UiElement(const std::string_view content,
                               std::unique_ptr<ViewModel> viewModel);
  CompositionBuilder CreateDrawList(std::unique_ptr<DrawList> drawList);

 private:
  ActorKeyGenerator actorKeyGenerator_;
  game::Camera& camera_;
  SpawnDependencies spawnDependencies_;
};

#endif  // SRC_ENGINE_COMPOSITION_SCENECOMPOSER_H_
