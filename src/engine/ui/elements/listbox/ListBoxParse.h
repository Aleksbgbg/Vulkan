#ifndef SRC_ENGINE_UI_ELEMENTS_LISTBOX_LISTBOXPARSE_H_
#define SRC_ENGINE_UI_ELEMENTS_LISTBOX_LISTBOXPARSE_H_

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

#endif  // SRC_ENGINE_UI_ELEMENTS_LISTBOX_LISTBOXPARSE_H_
