#ifndef SRC_ENGINE_UI_ELEMENTS_LISTBOX_LISTBOXLAYOUT_H_
#define SRC_ENGINE_UI_ELEMENTS_LISTBOX_LISTBOXLAYOUT_H_

#include <optional>
#include <vector>

#include "core/math/Rect.h"

struct ListBoxLayout {
  struct Header {
    std::optional<Rectu> accent;
    Rectu area;
  };

  Rectu headerArea;
  std::vector<Header> headers;
  Rectu contentArea;
};

#endif  // SRC_ENGINE_UI_ELEMENTS_LISTBOX_LISTBOXLAYOUT_H_
