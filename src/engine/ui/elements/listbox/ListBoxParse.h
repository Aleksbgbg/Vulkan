#ifndef VULKAN_SRC_ENGINE_UI_ELEMENTS_LISTBOX_LISTBOXPARSE_H_
#define VULKAN_SRC_ENGINE_UI_ELEMENTS_LISTBOX_LISTBOXPARSE_H_

#include "engine/ui/Element.h"
#include "engine/ui/Margin.h"
#include "engine/ui/Orientation.h"
#include "engine/ui/TextCase.h"

struct ListBoxParse {
  Orientation orientation;
  TextCase textCase;
  ElementList elements;
  Margin margin;
};

#endif  // VULKAN_SRC_ENGINE_UI_ELEMENTS_LISTBOX_LISTBOXPARSE_H_
