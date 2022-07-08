#ifndef SRC_ENGINE_BEHAVIOUR_UPDATECONTEXT_H_
#define SRC_ENGINE_BEHAVIOUR_UPDATECONTEXT_H_

#include "game/Controls.h"

struct UpdateContext {
  float deltaTime;
  Controls& controls;
};

#endif  // SRC_ENGINE_BEHAVIOUR_UPDATECONTEXT_H_
