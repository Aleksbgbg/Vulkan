#ifndef VULKAN_SRC_GAME_UI_ELEMENTS_LISTBOX_LISTBOXLAYOUT_H_
#define VULKAN_SRC_GAME_UI_ELEMENTS_LISTBOX_LISTBOXLAYOUT_H_

#include <optional>
#include <vector>

#include "general/geometry/Rect.h"

struct ListBoxLayout {
  struct Header {
    std::optional<Rectu> accent;
    Rectu area;
  };

  Rectu headerArea;
  std::vector<Header> headers;
  Rectu contentArea;
};

#endif  // VULKAN_SRC_GAME_UI_ELEMENTS_LISTBOX_LISTBOXLAYOUT_H_
