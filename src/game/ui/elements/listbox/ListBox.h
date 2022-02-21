#ifndef VULKAN_SRC_GAME_UI_ELEMENTS_LISTBOX_LISTBOX_H_
#define VULKAN_SRC_GAME_UI_ELEMENTS_LISTBOX_LISTBOX_H_

#include "ListBoxElement.h"
#include "core/math/Rect.h"
#include "game/ui/ElementContainer.h"
#include "game/ui/Margin.h"
#include "game/ui/Orientation.h"
#include "game/ui/TextCase.h"

struct ListBox {
  Orientation orientation;
  TextCase textCase;
  Margin margin;
  Rectu contentArea;
  ElementContainer<ListBoxElement> elements;
  bool isFocused;
};

#endif  // VULKAN_SRC_GAME_UI_ELEMENTS_LISTBOX_LISTBOX_H_
