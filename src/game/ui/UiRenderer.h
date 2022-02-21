#ifndef VULKAN_SRC_GAME_UI_UIRENDERER_H_
#define VULKAN_SRC_GAME_UI_UIRENDERER_H_

#include "Canvas.h"
#include "core/math/Rect.h"
#include "game/ui/SelectionState.h"
#include "game/ui/elements/gridlist/GridList.h"
#include "game/ui/elements/listbox/ListBox.h"
#include "game/ui/elements/slider/Slider.h"
#include "game/ui/elements/togglebox/ToggleBox.h"

class UiRenderer {
 public:
  UiRenderer(const Canvas& canvas, Rectu renderArea);

  void Render(const ListBox& listBox) const;
  void Render(const GridList& gridList) const;
  void Render(const Slider& slider) const;
  void Render(const ToggleBox& toggleBox) const;

 private:
  UiRenderer(const Canvas& canvas, Rectu renderArea,
             SelectionState inheritedSelection);

  UiRenderer Partition(Rectu newRenderArea) const;
  UiRenderer Partition(Rectu newRenderArea,
                       SelectionState inheritedSelection) const;

 private:
  const Canvas& canvas_;
  Rectu renderArea_;
  SelectionState inheritedSelection_;
};

#endif  // VULKAN_SRC_GAME_UI_UIRENDERER_H_
