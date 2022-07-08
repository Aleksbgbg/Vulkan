#ifndef SRC_ENGINE_PROPERTY_TRAITS_DEFAULT_ALWAYSVISIBLE_H_
#define SRC_ENGINE_PROPERTY_TRAITS_DEFAULT_ALWAYSVISIBLE_H_

#include "engine/property/traits/Visible.h"

class AlwaysVisible : public Visible {
 public:
  bool IsVisible() const override;
};

#endif  // SRC_ENGINE_PROPERTY_TRAITS_DEFAULT_ALWAYSVISIBLE_H_
