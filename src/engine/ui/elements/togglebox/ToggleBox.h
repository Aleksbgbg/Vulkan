#ifndef VULKAN_SRC_ENGINE_UI_ELEMENTS_TOGGLEBOX_TOGGLEBOX_H_
#define VULKAN_SRC_ENGINE_UI_ELEMENTS_TOGGLEBOX_TOGGLEBOX_H_

#include "ToggleBoxElement.h"
#include "ToggleBoxLayout.h"
#include "engine/ui/ElementContainer.h"

struct ToggleBox {
  ToggleBoxLayout layout;
  ElementContainer<ToggleBoxElement> elements;
};

#endif  // VULKAN_SRC_ENGINE_UI_ELEMENTS_TOGGLEBOX_TOGGLEBOX_H_
