#ifndef VULKAN_SRC_GAME_CONTROLS_H
#define VULKAN_SRC_GAME_CONTROLS_H

#include <unordered_map>
#include <unordered_set>

#include "general/geometry/Rect.h"
#include "general/windowing/input/Keyboard.h"
#include "general/windowing/input/Mouse.h"

enum class Control {
  Accelerate = 1,
  Decelerate,
  ReverseView,
  MoveNpc,
  SpawnPlayer,
};

enum class Axis {
  Movement = 1,
  View,
};

class Controls {
 public:
  bool IsControlActive(const Control control) const;
  glm::vec2 AxisValue(const Axis axis) const;

  void Update(const Keyboard& keyboard, const Mouse& mouse,
              const glm::vec2 windowSize);

 private:
  std::unordered_set<Control> activeControls_;
  std::unordered_map<Axis, glm::vec2> axisValues_;
};

#endif  // VULKAN_SRC_GAME_CONTROLS_H
