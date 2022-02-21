#ifndef VULKAN_SRC_ENGINE_UI_LAYOUTCALCULATOR_H_
#define VULKAN_SRC_ENGINE_UI_LAYOUTCALCULATOR_H_

#include "core/math/Rect.h"
#include "core/text/FontAtlas.h"
#include "engine/ui/elements/gridlist/GridList.h"
#include "engine/ui/elements/gridlist/GridListLayout.h"
#include "engine/ui/elements/listbox/ListBox.h"
#include "engine/ui/elements/listbox/ListBoxLayout.h"
#include "engine/ui/elements/slider/Slider.h"
#include "engine/ui/elements/slider/SliderLayout.h"
#include "engine/ui/elements/togglebox/ToggleBox.h"
#include "engine/ui/elements/togglebox/ToggleBoxLayout.h"

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

#endif  // VULKAN_SRC_ENGINE_UI_LAYOUTCALCULATOR_H_
