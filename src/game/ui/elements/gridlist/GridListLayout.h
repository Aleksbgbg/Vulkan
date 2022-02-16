#ifndef VULKAN_SRC_GAME_UI_ELEMENTS_GRIDLIST_GRIDLISTLAYOUT_H_
#define VULKAN_SRC_GAME_UI_ELEMENTS_GRIDLIST_GRIDLISTLAYOUT_H_

#include <vector>

#include "general/geometry/Rect.h"

struct GridListLayout {
  struct Row {
    Rectu rowArea;
    Rectu titleArea;
    Rectu contentArea;
  };

  std::vector<Row> rows;
};

#endif  // VULKAN_SRC_GAME_UI_ELEMENTS_GRIDLIST_GRIDLISTLAYOUT_H_
