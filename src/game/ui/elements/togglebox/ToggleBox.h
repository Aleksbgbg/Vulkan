#ifndef VULKAN_SRC_GAME_UI_ELEMENTS_TOGGLEBOX_TOGGLEBOX_H_
#define VULKAN_SRC_GAME_UI_ELEMENTS_TOGGLEBOX_TOGGLEBOX_H_

#include "ToggleBoxElement.h"
#include "ToggleBoxLayout.h"
#include "game/ui/ElementContainer.h"

struct ToggleBox {
  ToggleBoxLayout layout;
  ElementContainer<ToggleBoxElement> elements;
};

#endif  // VULKAN_SRC_GAME_UI_ELEMENTS_TOGGLEBOX_TOGGLEBOX_H_
