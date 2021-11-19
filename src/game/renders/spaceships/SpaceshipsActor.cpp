#include "SpaceshipsActor.h"

#include "MainPlayer.h"
#include "OtherPlayer.h"

SpaceshipsActor::SpaceshipsActor(SpaceshipMesh mesh, Camera& camera,
                                 ParticleController& particleController)
    : mesh_(std::move(mesh)),
      camera_(camera),
      particleController_(particleController),
      spaceships_(),
      systemInputPlayerController_(),
      noMovementPlayerController_() {}

void SpaceshipsActor::UpdateModel(const UpdateContext& context) {
  systemInputPlayerController_.UpdateModel(context);

  if (spaceships_.size() == 0) {
    spaceships_.push_back(std::make_unique<MainPlayer>(
        SpaceshipModel(mesh_, particleController_.CreateParticleStream()),
        camera_, systemInputPlayerController_));
  }

  for (const std::unique_ptr<Actor>& actor : spaceships_) {
    actor->UpdateModel(context);
  }

  if (context.controls.IsControlActive(Control::SpawnPlayer)) {
    spaceships_.push_back(std::make_unique<OtherPlayer>(
        SpaceshipModel(mesh_, particleController_.CreateParticleStream()),
        noMovementPlayerController_));
  }
}

const Mesh& SpaceshipsActor::GetMesh() const {
  return mesh_;
}

void SpaceshipsActor::Render(const MeshRenderer& renderer) const {
  for (const std::unique_ptr<Actor>& actor : spaceships_) {
    actor->Render(renderer);
  }
}
