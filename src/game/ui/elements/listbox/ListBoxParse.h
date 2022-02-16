#ifndef VULKAN_SRC_GAME_UI_ELEMENTS_LISTBOX_LISTBOXPARSE_H_
#define VULKAN_SRC_GAME_UI_ELEMENTS_LISTBOX_LISTBOXPARSE_H_

#include "game/ui/Element.h"
#include "game/ui/Margin.h"
#include "game/ui/Orientation.h"
#include "game/ui/TextCase.h"

struct ListBoxParse {
  Orientation orientation;
  TextCase textCase;
  ElementList elements;
  Margin margin;
};

#endif  // VULKAN_SRC_GAME_UI_ELEMENTS_LISTBOX_LISTBOXPARSE_H_
