#ifndef VULKAN_SRC_ENGINE_BEHAVIOUR_UPDATECONTEXT_H_
#define VULKAN_SRC_ENGINE_BEHAVIOUR_UPDATECONTEXT_H_

#include "game/Controls.h"

struct UpdateContext {
  float deltaTime;
  Controls& controls;
};

#endif  // VULKAN_SRC_ENGINE_BEHAVIOUR_UPDATECONTEXT_H_