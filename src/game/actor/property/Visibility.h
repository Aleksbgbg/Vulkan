#ifndef VULKAN_SRC_GAME_ACTOR_PROPERTY_VISIBILITY_H_
#define VULKAN_SRC_GAME_ACTOR_PROPERTY_VISIBILITY_H_

#include "Property.h"
#include "PropertyKey.h"
#include "game/Visible.h"

class Visibility : public Property, public Visible {
 public:
  constexpr static PropertyKey Key() {
    return PropertyKey::Visibility;
  }

  Visibility();

  bool IsVisible() const override;

  void ToggleVisibility();
  void MakeVisible();
  void MakeInvisible();
  void SetIsVisible(bool isVisible);

 private:
  bool isVisible_;
};

#endif  // VULKAN_SRC_GAME_ACTOR_PROPERTY_VISIBILITY_H_
