#ifndef SRC_ENGINE_PROPERTY_TYPE_VISIBILITY_H_
#define SRC_ENGINE_PROPERTY_TYPE_VISIBILITY_H_

#include "engine/property/Property.h"
#include "engine/property/PropertyKey.h"
#include "engine/property/traits/Visible.h"

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

#endif  // SRC_ENGINE_PROPERTY_TYPE_VISIBILITY_H_
