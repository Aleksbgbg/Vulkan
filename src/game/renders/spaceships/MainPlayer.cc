#include "MainPlayer.h"

#include "general/math/math.h"

MainPlayer::MainPlayer(SpaceshipModel spaceshipModel, Camera& camera,
                       const PlayerController& playerController)
    : spaceshipModel_(std::move(spaceshipModel)),
      camera_(camera),
      playerController_(playerController) {}

void MainPlayer::UpdateModel(const UpdateContext& context) {
  const glm::vec3 velocity = playerController_.GetVelocity();
  const glm::vec3 rotation = playerController_.GetRotation();

  glm::vec3 lookAround =
      glm::vec3(context.controls.AxisValue(Axis::View) * 2.0f, 0.0f);
  lookAround.x = CoerceToRange(lookAround.x, -QuarterTurn, QuarterTurn);

  spaceshipModel_.Rotate(rotation);
  spaceshipModel_.Move(velocity, context.deltaTime);

  glm::vec3* const modelPosition = spaceshipModel_.Position();

  const glm::mat4 cameraTransform =
      glm::translate(glm::mat4(1.0f), *modelPosition) *
      glm::toMat4(glm::quat(rotation)) * glm::toMat4(glm::quat(lookAround));

  const bool reverseView =
      context.controls.IsControlActive(Control::ReverseView);
  camera_.SetView(
      {.position =
           cameraTransform * glm::vec4(0.0f, 1.0f,
                                       reverseView ? spaceshipModel_.Size().z
                                                   : -spaceshipModel_.Size().z,
                                       1.0f),
       .lookAt = glm::vec4(*modelPosition + glm::vec3(0.0f, 0.5f, 0.0f), 1.0f),
       .rotation = rotation,
       .reverse = reverseView});
}

const Mesh& MainPlayer::GetMesh() const {
  return spaceshipModel_.GetMesh();
}

void MainPlayer::Render(const MeshRenderer& renderer) const {
  spaceshipModel_.Render(renderer);
}
