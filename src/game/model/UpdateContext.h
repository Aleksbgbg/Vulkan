#ifndef VULKAN_SRC_GAME_MODEL_UPDATECONTEXT_H
#define VULKAN_SRC_GAME_MODEL_UPDATECONTEXT_H

#include "general/windowing/input/Keyboard.h"

struct UpdateContext {
  float deltaTime;
  Keyboard& keyboard;
};

#endif  // VULKAN_SRC_GAME_MODEL_UPDATECONTEXT_H
