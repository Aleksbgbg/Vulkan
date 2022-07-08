#ifndef SRC_ENGINE_UI_ELEMENTS_TOGGLEBOX_TOGGLEBOX_H_
#define SRC_ENGINE_UI_ELEMENTS_TOGGLEBOX_TOGGLEBOX_H_

#include "ToggleBoxElement.h"
#include "ToggleBoxLayout.h"
#include "engine/ui/ElementContainer.h"

struct ToggleBox {
  ToggleBoxLayout layout;
  ElementContainer<ToggleBoxElement> elements;
};

#endif  // SRC_ENGINE_UI_ELEMENTS_TOGGLEBOX_TOGGLEBOX_H_
