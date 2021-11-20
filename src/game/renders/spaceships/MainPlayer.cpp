#include "MainPlayer.h"

#include "general/math/math.h"

MainPlayer::MainPlayer(SpaceshipModel spaceshipModel, Camera& camera,
                       const PlayerController& playerController)
    : spaceshipModel_(std::move(spaceshipModel)),
      camera_(camera),
      playerController_(playerController) {}

void MainPlayer::UpdateModel(const UpdateContext& context) {
  const bool isMoving = playerController_.IsMoving();
  const glm::vec3 position = playerController_.GetPosition();
  const glm::vec3 rotation = playerController_.GetRotation();

  glm::vec3 lookAround =
      glm::vec3(context.controls.AxisValue(Axis::View) * 2.0f, 0.0f);
  lookAround.x = CoerceToRange(lookAround.x, -QuarterTurn, QuarterTurn);

  spaceshipModel_.SetIsMoving(isMoving);
  spaceshipModel_.SetRotation(rotation);
  spaceshipModel_.SetPosition(position);
  spaceshipModel_.Update();

  const glm::mat4 cameraTransform = glm::translate(glm::mat4(1.0f), position) *
                                    glm::toMat4(glm::quat(rotation)) *
                                    glm::toMat4(glm::quat(lookAround));

  const bool reverseView =
      context.controls.IsControlActive(Control::ReverseView);
  camera_.SetView(
      {.position =
           cameraTransform * glm::vec4(0.0f, 1.0f,
                                       reverseView ? spaceshipModel_.Size().z
                                                   : -spaceshipModel_.Size().z,
                                       1.0f),
       .lookAt = glm::vec4(position + glm::vec3(0.0f, 0.5f, 0.0f), 1.0f),
       .rotation = rotation,
       .reverse = reverseView});
}

const Mesh& MainPlayer::GetMesh() const {
  return spaceshipModel_.GetMesh();
}

void MainPlayer::Render(const MeshRenderer& renderer) const {
  spaceshipModel_.Render(renderer);
}
