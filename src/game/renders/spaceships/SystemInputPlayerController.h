#ifndef VULKAN_SRC_GAME_RENDERS_SPACESHIPS_SYSTEMINPUTPLAYERCONTROLLER_H
#define VULKAN_SRC_GAME_RENDERS_SPACESHIPS_SYSTEMINPUTPLAYERCONTROLLER_H

#include "PlayerController.h"
#include "game/Controls.h"
#include "game/model/Updatable.h"

class SystemInputPlayerController : public PlayerController, public Updatable {
 public:
  SystemInputPlayerController();

  bool IsMoving() const override;
  glm::vec3 GetRotation() const override;
  glm::vec3 GetPosition() const override;

  void UpdateModel(const UpdateContext& context) override;

 private:
  bool moving_;
  glm::vec3 velocity_;
  glm::vec3 rotation_;
  glm::vec3 position_;
};

#endif  // VULKAN_SRC_GAME_RENDERS_SPACESHIPS_SYSTEMINPUTPLAYERCONTROLLER_H
