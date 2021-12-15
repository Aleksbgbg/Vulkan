#include "SceneComposer.h"

#include "game/behaviours/CameraController.h"

SceneComposer::SceneComposer(ActorConsumer& actorConsumer, Renderer& renderer,
                             sys::Sound& sound, game::Camera& camera)
    : actorConsumer_(actorConsumer),
      renderer_(renderer),
      sound_(sound),
      camera_(camera) {}

MeshHandle SceneComposer::LoadMesh(const RenderType renderType,
                                   const MeshLoadParams& meshLoadParams) {
  return renderer_.LoadMesh(renderType, meshLoadParams);
}

CompositionBuilder SceneComposer::Actor() {
  return CompositionBuilder::ForActor(actorConsumer_, renderer_, sound_);
}

CompositionBuilder SceneComposer::Camera() {
  return std::move(
      CompositionBuilder::ForActor(actorConsumer_, renderer_, sound_)
          .Attach([this]() {
            class CameraBehaviourFactory : public BehaviourFactory {
             public:
              CameraBehaviourFactory(game::Camera& camera) : camera_(camera) {}

              std::unique_ptr<Behaviour> operator()(
                  const game::Actor* parent, game::Actor& actor) override {
                return std::make_unique<CameraController>(
                    camera_, parent->RetrieveProperty<Transform>());
              }

             private:
              game::Camera& camera_;
            };

            return std::make_unique<CameraBehaviourFactory>(this->camera_);
          }));
}

CompositionBuilder SceneComposer::ParticleSystem(
    const ParticleBehaviour behaviour) {
  return CompositionBuilder::ForParticleSystem(actorConsumer_, renderer_,
                                               sound_, behaviour);
}
