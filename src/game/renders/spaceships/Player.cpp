#include "Player.h"

Player::Player(SpaceshipMesh mesh, Camera& camera, const Window& window)
    : spaceshipModel(std::move(mesh)), camera(camera), window(window) {}

void Player::UpdateModel(const UpdateContext& context) {
  glm::vec3 movement(0.0f);
  if (context.keyboard.IsKeyDown(SDLK_a)) {
    movement.x += 1.0f;
  }
  if (context.keyboard.IsKeyDown(SDLK_d)) {
    movement.x -= 1.0f;
  }
  if (context.keyboard.IsKeyDown(SDLK_w)) {
    movement.y += 1.0f;
  }
  if (context.keyboard.IsKeyDown(SDLK_s)) {
    movement.y -= 1.0f;
  }
  if (context.keyboard.IsKeyDown(SDLK_q)) {
    movement.z -= 1.0f;
  }
  if (context.keyboard.IsKeyDown(SDLK_e)) {
    movement.z += 1.0f;
  }

  spaceshipModel.Move(movement, context.deltaTime);

  const Keyboard& keyboard = window.GetKeyboard();
  const Mouse& mouse = window.GetMouse();

  glm::vec3* const modelPosition = spaceshipModel.Position();

  glm::mat4 cameraTransform(1.0f);
  cameraTransform = glm::translate(cameraTransform, *modelPosition);

  const bool reverseView = keyboard.IsKeyDown(SDLK_c);

  glm::vec2 mouseDelta = mouse.GetPositionDeltaFromRightClick();

  if (reverseView) {
    mouseDelta.y = -mouseDelta.y;
  }

  const Rectf windowRect = window.GetRect();

  cameraTransform = glm::rotate(
      cameraTransform, glm::pi<float>() * (-mouseDelta.x / windowRect.width),
      glm::vec3(0.0f, 1.0f, 0.0f));
  cameraTransform = glm::rotate(
      cameraTransform,
      glm::pi<float>() *
          std::max(-0.499f, std::min(mouseDelta.y / windowRect.height, 0.499f)),
      glm::vec3(1.0f, 0.0f, 0.0f));

  if (reverseView) {
    cameraTransform = glm::rotate(cameraTransform, glm::pi<float>(),
                                  glm::vec3(0.0f, 1.0f, 0.0f));
  }

  camera.SetView(
      {.position = cameraTransform *
                   glm::vec4(0.0f, 0.0f, -spaceshipModel.Size().z, 1.0f),
       .lookAt = *modelPosition});
}

const Mesh& Player::GetMesh() const {
  return spaceshipModel.GetMesh();
}

void Player::Render(const MeshRenderer& renderer) const {
  spaceshipModel.Render(renderer);
}
