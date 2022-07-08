#ifndef SRC_ENGINE_UI_ELEMENTS_TOGGLEBOX_TOGGLEBOXPARSE_H_
#define SRC_ENGINE_UI_ELEMENTS_TOGGLEBOX_TOGGLEBOXPARSE_H_

#include "core/types.h"
#include "engine/ui/Bound.h"
#include "engine/ui/Element.h"

struct ToggleBoxParse {
  ElementList elements;
  Bound<u32>& selectedIndexBinder;
};

#endif  // SRC_ENGINE_UI_ELEMENTS_TOGGLEBOX_TOGGLEBOXPARSE_H_
