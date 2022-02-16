#ifndef VULKAN_SRC_GAME_UI_LAYOUTCALCULATOR_H_
#define VULKAN_SRC_GAME_UI_LAYOUTCALCULATOR_H_

#include "game/ui/elements/gridlist/GridList.h"
#include "game/ui/elements/gridlist/GridListLayout.h"
#include "game/ui/elements/listbox/ListBox.h"
#include "game/ui/elements/listbox/ListBoxLayout.h"
#include "game/ui/elements/slider/Slider.h"
#include "game/ui/elements/slider/SliderLayout.h"
#include "game/ui/elements/togglebox/ToggleBox.h"
#include "game/ui/elements/togglebox/ToggleBoxLayout.h"
#include "general/geometry/Rect.h"
#include "general/text/FontAtlas.h"

class LayoutCalculator {
 public:
  LayoutCalculator(const FontAtlas& fontAtlas, Rectu layoutArea);

  ListBoxLayout CalculateLayout(const ListBox& listBox) const;
  GridListLayout CalculateLayout(const GridList& gridList) const;
  ToggleBoxLayout CalculateLayout(const ToggleBox& toggleBox) const;
  SliderLayout CalculateLayout(const Slider& slider) const;

  LayoutCalculator Partition(Rectu area) const;

 private:
  const FontAtlas& fontAtlas_;
  Rectu layoutArea_;
};

#endif  // VULKAN_SRC_GAME_UI_LAYOUTCALCULATOR_H_
