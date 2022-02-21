#include "GridListUiElement.h"

GridList ToGridList(GridListParse& parse) {
  return {.elements = ElementContainer<GridListElement>(parse.elements)};
}

GridListUiElement::GridListUiElement(GridListParse gridListParse)
    : gridList_(ToGridList(gridListParse)) {}

void GridListUiElement::OnAcquiredFocus() {
  gridList_.isFocused = true;
}

void GridListUiElement::OnLostFocus() {
  gridList_.isFocused = false;
}

void GridListUiElement::ProcessAction(FocusTracker& focusTracker,
                                      const Action& action) {
  if (action.type == Action::Type::Hover) {
    const Rectu::Vec point(action.hover.x, action.hover.y);

    gridList_.elements.ClearHover();

    for (ContainerElement<GridListElement>& element : gridList_.elements) {
      if (element->row.rowArea.ContainsPoint(point)) {
        element.Hover();
        return;
      }
    }
  } else if (action.type == Action::Type::Click) {
    const Rectu::Vec point(action.click.x, action.click.y);

    for (ContainerElement<GridListElement>& element : gridList_.elements) {
      if (element->row.rowArea.ContainsPoint(point)) {
        element.Select();
        focusTracker.AcquireFocus(this);
        return;
      }
    }
  } else if (action.type == Action::Type::Move) {
    if (action.move.direction == Action::Move::Direction::Up) {
      gridList_.elements.SelectPrevious();
      return;
    } else if (action.move.direction == Action::Move::Direction::Down) {
      gridList_.elements.SelectNext();
      return;
    }
  } else if (action.type == Action::Type::Select) {
    if (!focusTracker.HasFocus(this)) {
      focusTracker.AcquireFocus(this);
      return;
    }
  } else if (action.type == Action::Type::Deselect) {
    if (focusTracker.HasFocus(this)) {
      focusTracker.RenounceFocus(this);
      return;
    }
  }

  gridList_.elements.ActiveUiElement().ProcessAction(focusTracker, action);
}

void GridListUiElement::CalculateLayout(
    const LayoutCalculator& layoutCalculator) {
  const GridListLayout layout = layoutCalculator.CalculateLayout(gridList_);

  auto rowIterator = std::begin(layout.rows);

  for (ContainerElement<GridListElement>& element : gridList_.elements) {
    element->row = *rowIterator;
    element.GetUiElement().CalculateLayout(
        layoutCalculator.Partition(rowIterator->contentArea));

    rowIterator = std::next(rowIterator);
  }
}

void GridListUiElement::Render(const UiRenderer& renderer) const {
  renderer.Render(gridList_);
}
