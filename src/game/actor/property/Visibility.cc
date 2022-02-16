#include "Visibility.h"

Visibility::Visibility() : isVisible_(true) {}

bool Visibility::IsVisible() const {
  return isVisible_;
}

void Visibility::ToggleVisibility() {
  isVisible_ = !isVisible_;
}

void Visibility::MakeVisible() {
  isVisible_ = true;
}

void Visibility::MakeInvisible() {
  isVisible_ = false;
}

void Visibility::SetIsVisible(const bool isVisible) {
  isVisible_ = isVisible;
}
