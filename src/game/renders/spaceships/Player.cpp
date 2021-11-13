#include "Player.h"

Player::Player(SpaceshipMesh mesh, Camera& camera, const Window& window,
               ParticleController& particleController)
    : spaceshipModel(std::move(mesh)),
      camera(camera),
      window(window),
      particleController(particleController),
      velocity(0.0f),
      rotation(0.0f) {}

float OffsetValueByDeadzone(const float value, const float deadzone) {
  if (std::abs(value) < deadzone) {
    return 0.0f;
  }

  if (value < 0.0f) {
    return value + deadzone;
  }

  return value - deadzone;
}

glm::vec2 CalculateMouseMovement(const Mouse& mouse, const Rectf& windowRect) {
  glm::vec2 movement =
      mouse.GetPosition() -
      (glm::vec2(windowRect.Width(), windowRect.Height()) / 2.0f);
  movement.x = -movement.x;

  constexpr float Deadzone = 20.0f;
  movement.x = OffsetValueByDeadzone(movement.x, Deadzone);
  movement.y = OffsetValueByDeadzone(movement.y, Deadzone);

  return movement;
}

float CoerceToRange(const float value, const float min, const float max) {
  return std::min(max, std::max(min, value));
}

float CalculateRotationFromDistanceAsFraction(const float distance,
                                              const float max) {
  constexpr float MaxRotation = 0.25f;
  return glm::pi<float>() *
         CoerceToRange(distance / max, -MaxRotation, MaxRotation);
}

void Player::UpdateModel(const UpdateContext& context) {
  float acceleration = 0.0f;
  if (context.keyboard.IsKeyDown(SDLK_w)) {
    acceleration += 1.0f;
  }
  if (context.keyboard.IsKeyDown(SDLK_s)) {
    acceleration -= 1.0f;
  }

  velocity.z += acceleration * context.deltaTime;
  velocity.z = CoerceToRange(velocity.z, 0.0f, 1.0f);

  const Rectf windowRect = window.GetRect();
  const glm::vec2 mouseMovement =
      CalculateMouseMovement(window.GetMouse(), windowRect);
  glm::vec3* const modelPosition = spaceshipModel.Position();

  glm::mat4 cameraTransform(1.0f);
  cameraTransform = glm::translate(cameraTransform, *modelPosition);

  const bool reverseView = context.keyboard.IsKeyDown(SDLK_c);

  const glm::vec2 rotationDelta =
      glm::vec2(CalculateRotationFromDistanceAsFraction(mouseMovement.x,
                                                        windowRect.Width()),
                CalculateRotationFromDistanceAsFraction(mouseMovement.y,
                                                        windowRect.Height())) *
      context.deltaTime;

  rotation.x += rotationDelta.x;
  rotation.y += rotationDelta.y;

  const glm::vec3 frameVelocity =
      glm::rotate(
          glm::rotate(glm::mat4(1.0f), rotation.x, glm::vec3(0.0f, 1.0f, 0.0f)),
          rotation.y, glm::vec3(1.0f, 0.0f, 0.0f)) *
      glm::vec4(velocity, 0.0f);

  constexpr glm::vec3 ExhaustPositionTopLeft = glm::vec3(13.0f, 6.0f, 46.0f);
  constexpr glm::vec3 ExhaustPositionBottomRight =
      glm::vec3(21.0f, 7.0f, 46.0f);
  constexpr glm::vec3 ShipDimensions = glm::vec3(34.0f, 16.0f, 56.0f);
  constexpr glm::vec3 ExhaustPositionTopLeftNormalized =
      ExhaustPositionTopLeft / ShipDimensions;
  constexpr glm::vec3 ExhaustPositionBottomRightNormalized =
      ExhaustPositionBottomRight / ShipDimensions;

  const glm::vec3 halfSpaceshipSize = spaceshipModel.Size() / 2.0f;
  const glm::vec3 exhaustPositionTopLeft =
      (ExhaustPositionTopLeftNormalized * spaceshipModel.Size()) -
      halfSpaceshipSize;
  const glm::vec3 exhaustPositionBottomRight =
      (ExhaustPositionBottomRightNormalized * spaceshipModel.Size()) -
      halfSpaceshipSize;

  particleController.SetSpawnArea(
      Rectf::FromPoints(exhaustPositionTopLeft, exhaustPositionBottomRight));
  particleController.SetTransform(
      glm::translate(glm::mat4(1.0f), *spaceshipModel.Position()) *
      glm::toMat4(glm::quat(glm::vec3(rotation.y, rotation.x, 0.0f))) *
      glm::translate(glm::mat4(1.0f),
                     glm::vec3(0.0f, 0.0f, -exhaustPositionTopLeft.z)));

  spaceshipModel.Rotate(rotation);
  const bool moved = spaceshipModel.Move(frameVelocity, context.deltaTime);

  particleController.SetEnabled(moved);

  cameraTransform = glm::rotate(
      glm::rotate(cameraTransform, rotation.x, glm::vec3(0.0f, 1.0f, 0.0f)),
      rotation.y, glm::vec3(1.0f, 0.0f, 0.0f));

  camera.SetView(
      {.position =
           cameraTransform * glm::vec4(0.0f, 1.0f,
                                       reverseView ? spaceshipModel.Size().z
                                                   : -spaceshipModel.Size().z,
                                       1.0f),
       .lookAt = glm::vec4(*modelPosition + glm::vec3(0.0f, 0.5f, 0.0f), 1.0f),
       .rotation = rotation,
       .reverse = reverseView});
}

const Mesh& Player::GetMesh() const {
  return spaceshipModel.GetMesh();
}

void Player::Render(const MeshRenderer& renderer) const {
  spaceshipModel.Render(renderer);
}
