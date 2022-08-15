#include "GraphicalInterface.h"

GraphicalInterface::GraphicalInterface(Visibility& visibility)
    : visibility_(visibility), hideRequested_(false) {}

bool GraphicalInterface::IsVisible() const {
  return visibility_.IsVisible();
}

void GraphicalInterface::Show() {
  visibility_.MakeVisible();
}

void GraphicalInterface::Hide() {
  visibility_.MakeInvisible();
  hideRequested_ = false;
}

void GraphicalInterface::ToggleVisibility() {
  if (IsVisible()) {
    Hide();
  } else {
    Show();
  }
}

bool GraphicalInterface::HideRequested() const {
  return hideRequested_;
}

void GraphicalInterface::RequestHide() {
  hideRequested_ = true;
}
