#ifndef VULKAN_SRC_ENGINE_UI_UIRENDERER_H_
#define VULKAN_SRC_ENGINE_UI_UIRENDERER_H_

#include "Canvas.h"
#include "SelectionState.h"
#include "core/math/Rect.h"
#include "engine/ui/elements/gridlist/GridList.h"
#include "engine/ui/elements/listbox/ListBox.h"
#include "engine/ui/elements/slider/Slider.h"
#include "engine/ui/elements/togglebox/ToggleBox.h"

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

#endif  // VULKAN_SRC_ENGINE_UI_UIRENDERER_H_
