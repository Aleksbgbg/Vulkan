#include "UiRenderer.h"

#include "SelectionState.h"
#include "UiElement.h"

namespace {

static constexpr Color LIGHT_BLUE = {50.0f / 255.0f, 90.0f / 255.0f,
                                     106.0f / 255.0f, 1.0f};
static constexpr Color BLUE = {101.0f / 255.0f, 180.0f / 255.0f,
                               212.0f / 255.0f, 1.0f};

static constexpr Color HIGHLIGHT = {50.0f / 255.0f, 50.0f / 255.0f,
                                    50.0f / 255.0f, 0.5f};

Color BackgroundColor(const SelectionState selection) {
  static const std::unordered_map<SelectionState, Color> ColorsForSelection = {
      {SelectionState::Inactive, BLACK_BACKGROUND},
      {SelectionState::Hovered, HIGHLIGHT},
      {SelectionState::Selected, WHITE}};
  return ColorsForSelection.at(selection);
}

Color TextColor(const SelectionState selection) {
  static const std::unordered_map<SelectionState, Color> ColorsForSelection = {
      {SelectionState::Inactive, WHITE},
      {SelectionState::Hovered, WHITE},
      {SelectionState::Selected, BLACK}};
  return ColorsForSelection.at(selection);
}

SelectionState SelectionIfFocused(const bool isFocused,
                                  const SelectionState selection) {
  if (isFocused) {
    return selection;
  }
  return SelectionState::Inactive;
}

}  // namespace

UiRenderer::UiRenderer(const Canvas& canvas, const Rectu renderArea)
    : canvas_(canvas),
      renderArea_(renderArea),
      inheritedSelection_(SelectionState::Inactive) {}

UiRenderer::UiRenderer(const Canvas& canvas, const Rectu renderArea,
                       const SelectionState inheritedSelection)
    : canvas_(canvas),
      renderArea_(renderArea),
      inheritedSelection_(inheritedSelection) {}

void UiRenderer::Render(const ListBox& listBox) const {
  for (const ContainerElement<ListBoxElement>& element : listBox.elements) {
    const ListBoxLayout::Header& header = element->header;

    if (header.accent.has_value()) {
      canvas_.DrawRect(header.accent.value(), BLUE);
    }

    const SelectionState selection =
        SelectionIfFocused(listBox.isFocused, element.Selection());

    canvas_.DrawRect(header.area, BackgroundColor(selection));
    canvas_.Write(header.area, element.Title(), listBox.textCase,
                  TextAlignment::Center, TextColor(selection));
  }

  if (!listBox.isFocused) {
    const ContainerElement<ListBoxElement>* const hoveredElement =
        listBox.elements.HoveredElement();
    if (hoveredElement != nullptr) {
      canvas_.DrawRect((*hoveredElement)->header.area,
                       BackgroundColor(SelectionState::Hovered));
    }
  }

  listBox.elements.ActiveUiElement().Render(Partition(listBox.contentArea));
}

void UiRenderer::Render(const GridList& gridList) const {
  canvas_.DrawRect(renderArea_, BLACK_BACKGROUND);

  const ContainerElement<GridListElement>& selectedElement =
      gridList.elements.SelectedElement();
  const ContainerElement<GridListElement>* const hoveredElement =
      gridList.elements.HoveredElement();

  if (gridList.isFocused) {
    canvas_.DrawRect(selectedElement->row.rowArea, WHITE);
  }
  if ((hoveredElement != nullptr) &&
      !(gridList.isFocused && (&selectedElement == hoveredElement))) {
    canvas_.DrawRect((*hoveredElement)->row.rowArea, HIGHLIGHT);
  }

  for (const ContainerElement<GridListElement>& element : gridList.elements) {
    const SelectionState selection =
        SelectionIfFocused(gridList.isFocused, element.Selection());

    canvas_.Write(element->row.titleArea, element.Title(), TextCase::Regular,
                  TextAlignment::Left, TextColor(selection));

    element.GetUiElement().Render(Partition(element->row.titleArea, selection));
  }
}

void UiRenderer::Render(const Slider& slider) const {
  canvas_.DrawRect(slider.layout.background, BLUE);
  canvas_.DrawRect(slider.layout.highlight, LIGHT_BLUE);
}

void UiRenderer::Render(const ToggleBox& toggleBox) const {
  GlyphString glyphString =
      TO_GLYPH_STRING(toggleBox.elements.SelectedElement().Title());

  // Show left / right selection adorners
  if (inheritedSelection_ == SelectionState::Selected) {
    glyphString.insert(0, GLYPH_STRING_LITERAL("\uf104 "));
    glyphString += GLYPH_STRING_LITERAL(" \uf105");
  }

  canvas_.Write(toggleBox.layout.renderArea, glyphString, TextCase::Regular,
                TextAlignment::Right, TextColor(inheritedSelection_));
}

UiRenderer UiRenderer::Partition(const Rectu newRenderArea) const {
  return UiRenderer(canvas_, newRenderArea);
}

UiRenderer UiRenderer::Partition(
    const Rectu newRenderArea, const SelectionState inheritedSelection) const {
  return UiRenderer(canvas_, newRenderArea, inheritedSelection);
}
