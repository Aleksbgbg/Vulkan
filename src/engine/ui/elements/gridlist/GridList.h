#ifndef SRC_ENGINE_UI_ELEMENTS_GRIDLIST_GRIDLIST_H_
#define SRC_ENGINE_UI_ELEMENTS_GRIDLIST_GRIDLIST_H_

#include "GridListElement.h"
#include "engine/ui/ElementContainer.h"

struct GridList {
  bool isFocused;
  ElementContainer<GridListElement> elements;
};

#endif  // SRC_ENGINE_UI_ELEMENTS_GRIDLIST_GRIDLIST_H_
