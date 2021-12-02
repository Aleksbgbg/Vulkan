#ifndef VULKAN_SRC_GAME_MODEL_UPDATECONTEXT_H_
#define VULKAN_SRC_GAME_MODEL_UPDATECONTEXT_H_

#include "game/Controls.h"
#include "general/windowing/input/Keyboard.h"

struct UpdateContext {
  float deltaTime;
  Controls& controls;
};

#endif  // VULKAN_SRC_GAME_MODEL_UPDATECONTEXT_H_
