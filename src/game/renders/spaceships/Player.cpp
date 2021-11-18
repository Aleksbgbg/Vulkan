#include "Player.h"

#include "general/math/math.h"

static constexpr float HalfTurn = glm::pi<float>();
static constexpr float QuarterTurn = 0.5f * HalfTurn;
static constexpr float ThreeQuarterTurn = 1.5f * HalfTurn;
static constexpr float FullTurn = 2.0f * HalfTurn;

Player::Player(SpaceshipMesh mesh, Camera& camera, const wnd::Window& window,
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

  return movement;
}

float CalculateRotationFromDistanceAsFraction(const float distance,
                                              const float max) {
  constexpr float MaxRotation = 0.25f;
  return glm::pi<float>() *
         CoerceToRange(distance / max, -MaxRotation, MaxRotation);
}

bool Xor(const bool a, const bool b) {
  return (!a) != (!b);
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
  const glm::vec2 mouseMovementWithoutDeadzone =
      CalculateMouseMovement(window.GetMouse(), windowRect);
  constexpr float Deadzone = 20.0f;
  const glm::vec2 mouseMovement = glm::vec2(
      OffsetValueByDeadzone(mouseMovementWithoutDeadzone.x, Deadzone),
      OffsetValueByDeadzone(mouseMovementWithoutDeadzone.y, Deadzone));

  const bool reverseView = context.keyboard.IsKeyDown(SDLK_c);
  const bool freeView = context.keyboard.IsKeyDown(SDLK_LALT);

  glm::vec3 cameraViewAroundRotation(0.0f);

  if (freeView) {
    const glm::vec2 rotationForFrame =
        glm::vec2(mouseMovementWithoutDeadzone.x * 0.005f,
                  mouseMovementWithoutDeadzone.y * 0.005f);
    cameraViewAroundRotation.x +=
        CoerceToRange(rotationForFrame.y * 2.0f, -QuarterTurn, QuarterTurn);
    cameraViewAroundRotation.y += rotationForFrame.x * 2.0f;
  } else {
    const glm::vec2 rotationForFrame =
        glm::vec2(CalculateRotationFromDistanceAsFraction(mouseMovement.x,
                                                          windowRect.Width()),
                  CalculateRotationFromDistanceAsFraction(mouseMovement.y,
                                                          windowRect.Height()));
    const glm::vec2 rotationDelta = rotationForFrame * context.deltaTime;

    rotation.x += rotationDelta.x;
    rotation.y += rotationDelta.y;
  }

  const glm::vec3 frameVelocity =
      glm::rotate(
          glm::rotate(glm::mat4(1.0f), rotation.x, glm::vec3(0.0f, 1.0f, 0.0f)),
          rotation.y, glm::vec3(1.0f, 0.0f, 0.0f)) *
      glm::vec4(velocity, 0.0f);

  spaceshipModel.Rotate(rotation);
  const bool moved = spaceshipModel.Move(frameVelocity, context.deltaTime);

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

  particleController.SetEnabled(moved);

  glm::vec3* const modelPosition = spaceshipModel.Position();

  glm::mat4 cameraTransform(1.0f);
  cameraTransform = glm::translate(cameraTransform, *modelPosition);

  cameraTransform = glm::rotate(
      glm::rotate(cameraTransform, rotation.x, glm::vec3(0.0f, 1.0f, 0.0f)),
      rotation.y, glm::vec3(1.0f, 0.0f, 0.0f));
  cameraTransform *= glm::toMat4(glm::quat(cameraViewAroundRotation));

  const float horizontalRotationAbsoluteAngle =
      std::fmod(std::abs(cameraViewAroundRotation.y), FullTurn);

  camera.SetView(
      {.position =
           cameraTransform * glm::vec4(0.0f, 1.0f,
                                       reverseView ? spaceshipModel.Size().z
                                                   : -spaceshipModel.Size().z,
                                       1.0f),
       .lookAt = glm::vec4(*modelPosition + glm::vec3(0.0f, 0.5f, 0.0f), 1.0f),
       .rotation = rotation,
       .reverse = Xor(reverseView,
                      (ThreeQuarterTurn > horizontalRotationAbsoluteAngle) &&
                          (horizontalRotationAbsoluteAngle > QuarterTurn))});
}

const Mesh& Player::GetMesh() const {
  return spaceshipModel.GetMesh();
}

void Player::Render(const MeshRenderer& renderer) const {
  spaceshipModel.Render(renderer);
}
