#include "SpaceshipsActor.h"

#include "MainPlayer.h"
#include "OtherPlayer.h"

SpaceshipsActor::SpaceshipsActor(SpaceshipMesh mesh, Camera& camera,
                                 ParticleController& particleController,
                                 const PlayerController& mainPlayerController)
    : mainPlayerSpawned_(false),
      mesh_(std::move(mesh)),
      camera_(camera),
      particleController_(particleController),
      spaceships_(),
      mainPlayerController_(mainPlayerController),
      systemInputPlayerController_(),
      noMovementPlayerController_() {}

void SpaceshipsActor::UpdateModel(const UpdateContext& context) {
  systemInputPlayerController_.UpdateModel(context);

  if (!mainPlayerSpawned_) {
    spaceships_.push_back(std::make_unique<MainPlayer>(
        SpaceshipModel(mesh_, particleController_.CreateParticleStream()),
        camera_, mainPlayerController_));
    mainPlayerSpawned_ = true;
  }

  for (const std::unique_ptr<Actor>& actor : spaceships_) {
    actor->UpdateModel(context);
  }

  //  if (context.controls.IsControlActive(Control::SpawnPlayer)) {
  //    spaceships_.push_back(std::make_unique<OtherPlayer>(
  //        SpaceshipModel(mesh_, particleController_.CreateParticleStream()),
  //        systemInputPlayerController_));
  //  }
}

const Mesh& SpaceshipsActor::GetMesh() const {
  return mesh_;
}

void SpaceshipsActor::Render(const MeshRenderer& renderer) const {
  for (const std::unique_ptr<Actor>& actor : spaceships_) {
    actor->Render(renderer);
  }
}

void SpaceshipsActor::SpawnPlayer(const PlayerController& controller) {
  spaceships_.push_back(std::make_unique<OtherPlayer>(
      SpaceshipModel(mesh_, particleController_.CreateParticleStream()),
      controller));
}
