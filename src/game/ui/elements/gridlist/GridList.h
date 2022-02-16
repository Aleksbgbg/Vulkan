#ifndef VULKAN_SRC_GAME_UI_ELEMENTS_GRIDLIST_GRIDLIST_H_
#define VULKAN_SRC_GAME_UI_ELEMENTS_GRIDLIST_GRIDLIST_H_

#include "GridListElement.h"
#include "game/ui/ElementContainer.h"

struct GridList {
  bool isFocused;
  ElementContainer<GridListElement> elements;
};

#endif  // VULKAN_SRC_GAME_UI_ELEMENTS_GRIDLIST_GRIDLIST_H_
