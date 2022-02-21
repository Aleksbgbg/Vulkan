#ifndef VULKAN_SRC_GAME_UI_ELEMENTS_TOGGLEBOX_TOGGLEBOXPARSE_H_
#define VULKAN_SRC_GAME_UI_ELEMENTS_TOGGLEBOX_TOGGLEBOXPARSE_H_

#include "core/types.h"
#include "game/ui/Bound.h"
#include "game/ui/Element.h"

struct ToggleBoxParse {
  ElementList elements;
  Bound<u32>& selectedIndexBinder;
};

#endif  // VULKAN_SRC_GAME_UI_ELEMENTS_TOGGLEBOX_TOGGLEBOXPARSE_H_
