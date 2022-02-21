#ifndef VULKAN_SRC_ENGINE_UI_ELEMENTS_LISTBOX_LISTBOX_H_
#define VULKAN_SRC_ENGINE_UI_ELEMENTS_LISTBOX_LISTBOX_H_

#include "ListBoxElement.h"
#include "core/math/Rect.h"
#include "engine/ui/ElementContainer.h"
#include "engine/ui/Margin.h"
#include "engine/ui/Orientation.h"
#include "engine/ui/TextCase.h"

struct ListBox {
  Orientation orientation;
  TextCase textCase;
  Margin margin;
  Rectu contentArea;
  ElementContainer<ListBoxElement> elements;
  bool isFocused;
};

#endif  // VULKAN_SRC_ENGINE_UI_ELEMENTS_LISTBOX_LISTBOX_H_
