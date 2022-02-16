#ifndef VULKAN_SRC_GAME_CONTROLS_H_
#define VULKAN_SRC_GAME_CONTROLS_H_

#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "general/geometry/Rect.h"
#include "system/windowing/Window.h"

enum class Control {
  Accelerate = 1,
  Decelerate,
  ReverseView,
  SpawnPlayer,
  TiltLeft,
  TiltRight,
  Shoot,
  TogglePauseMenu,
};

enum class Axis {
  Movement = 1,
  View,
};

class Controls {
 public:
  Controls();

  bool IsControlActive(const Control control) const;
  glm::vec2 NormalizedAxisValue(const Axis axis) const;

  void Update(const sys::Window& window);

 private:
  struct ControlWithKey {
    Control control;
    SDL_KeyCode key;
  };
  std::vector<ControlWithKey> continuousKeyControls_;
  std::vector<ControlWithKey> singleKeyControls_;

  std::unordered_set<Control> activeControls_;
  std::unordered_map<Axis, glm::vec2> normalizedAxisValues_;
};

#endif  // VULKAN_SRC_GAME_CONTROLS_H_
