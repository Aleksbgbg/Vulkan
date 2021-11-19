#include "OtherPlayer.h"

OtherPlayer::OtherPlayer(SpaceshipModel spaceshipModel,
                         const PlayerController& playerController)
    : spaceshipModel_(std::move(spaceshipModel)),
      playerController_(playerController) {}

void OtherPlayer::UpdateModel(const UpdateContext& context) {
  spaceshipModel_.Rotate(playerController_.GetRotation());
  spaceshipModel_.Move(playerController_.GetVelocity(), context.deltaTime);
}

const Mesh& OtherPlayer::GetMesh() const {
  return spaceshipModel_.GetMesh();
}

void OtherPlayer::Render(const MeshRenderer& renderer) const {
  spaceshipModel_.Render(renderer);
}
