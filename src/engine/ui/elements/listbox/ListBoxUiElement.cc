#include "ListBoxUiElement.h"

ListBox ToListBox(ListBoxParse& parse) {
  return {.orientation = parse.orientation,
          .textCase = parse.textCase,
          .margin = parse.margin,
          .elements = ElementContainer<ListBoxElement>(parse.elements),
          .isFocused = false};
}

ListBoxUiElement::ListBoxUiElement(ListBoxParse listBoxParse)
    : listBox_(ToListBox(listBoxParse)) {}

void ListBoxUiElement::OnAcquiredFocus() {
  listBox_.isFocused = true;
}

void ListBoxUiElement::OnChildAcquiredFocus() {
  listBox_.isFocused = true;
}

void ListBoxUiElement::OnLostFocus() {
  listBox_.isFocused = false;
}

void ListBoxUiElement::ProcessAction(FocusTracker& focusTracker,
                                     const Action& action) {
  if (action.type == Action::Type::Hover) {
    const Rectu::Vec point(action.hover.x, action.hover.y);

    listBox_.elements.ClearHover();

    for (ContainerElement<ListBoxElement>& element : listBox_.elements) {
      if (element->header.area.ContainsPoint(point)) {
        element.Hover();
        return;
      }
    }
  } else if (action.type == Action::Type::Click) {
    const Rectu::Vec point(action.click.x, action.click.y);

    for (ContainerElement<ListBoxElement>& element : listBox_.elements) {
      if (element->header.area.ContainsPoint(point)) {
        element.Select();
        focusTracker.AcquireFocus(this);
        return;
      }
    }
  } else if (action.type == Action::Type::Move) {
    if (focusTracker.HasFocus(this)) {
      Action::Move::Direction previousDirection;
      Action::Move::Direction nextDirection;

      if (listBox_.orientation == Orientation::Horizontal) {
        previousDirection = Action::Move::Direction::Left;
        nextDirection = Action::Move::Direction::Right;
      } else {
        previousDirection = Action::Move::Direction::Up;
        nextDirection = Action::Move::Direction::Down;
      }

      if (action.move.direction == previousDirection) {
        listBox_.elements.SelectPrevious();
      } else if (action.move.direction == nextDirection) {
        listBox_.elements.SelectNext();
      }

      return;
    }
  } else if (action.type == Action::Type::Select) {
    if (!focusTracker.HasFocus(this) && !focusTracker.ChildHasFocus(this)) {
      focusTracker.AcquireFocus(this);
      return;
    }
  } else if (action.type == Action::Type::Deselect) {
    if (focusTracker.HasFocus(this)) {
      focusTracker.RenounceFocus(this);
      return;
    }
  }

  listBox_.elements.ActiveUiElement().ProcessAction(focusTracker, action);
}

void ListBoxUiElement::CalculateLayout(
    const LayoutCalculator& layoutCalculator) {
  const ListBoxLayout layout = layoutCalculator.CalculateLayout(listBox_);

  listBox_.contentArea = layout.contentArea;

  auto headerIterator = std::begin(layout.headers);

  for (ContainerElement<ListBoxElement>& element : listBox_.elements) {
    element->header = *headerIterator;
    headerIterator = std::next(headerIterator);
  }

  listBox_.elements.ActiveUiElement().CalculateLayout(
      layoutCalculator.Partition(layout.contentArea));
}

void ListBoxUiElement::Render(const UiRenderer& renderer) const {
  renderer.Render(listBox_);
}
