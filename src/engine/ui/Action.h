#ifndef VULKAN_SRC_ENGINE_UI_ACTION_H_
#define VULKAN_SRC_ENGINE_UI_ACTION_H_

#include "core/types.h"

struct Action {
  enum class Type {
    Hover,
    Click,
    Move,
    Select,
    Deselect,
  };

  struct Hover {
    u32 x;
    u32 y;
  };

  struct Click {
    u32 x;
    u32 y;
  };

  struct Move {
    enum class Direction {
      Up,
      Down,
      Left,
      Right,
    };

    Direction direction;
  };

  Type type;
  union {
    Hover hover;
    Click click;
    Move move;
  };
};

#endif  // VULKAN_SRC_ENGINE_UI_ACTION_H_
