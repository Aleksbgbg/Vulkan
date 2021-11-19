#include "PlayerSpaceships.h"

#include "Player.h"

PlayerSpaceships::PlayerSpaceships(SpaceshipMesh mesh, Camera& camera,
                                   const wnd::Window& window,
                                   ParticleController& particleController)
    : mesh_(std::move(mesh)),
      camera_(camera),
      window_(window),
      particleController_(particleController),
      spaceships_() {}

void PlayerSpaceships::UpdateModel(const UpdateContext& context) {
  if (spaceships_.size() == 0) {
    spaceships_.push_back(std::make_unique<Player>(
        mesh_, camera_, window_, particleController_.CreateParticleStream()));
  }

  for (const std::unique_ptr<Actor>& actor : spaceships_) {
    actor->UpdateModel(context);
  }

  if (context.keyboard.PressedKey(SDLK_g)) {
    spaceships_.push_back(std::make_unique<Player>(
        mesh_, camera_, window_, particleController_.CreateParticleStream()));
  }
}

const Mesh& PlayerSpaceships::GetMesh() const {
  return mesh_;
}

void PlayerSpaceships::Render(const MeshRenderer& renderer) const {
  for (const std::unique_ptr<Actor>& actor : spaceships_) {
    actor->Render(renderer);
  }
}
