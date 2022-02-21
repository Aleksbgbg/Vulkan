#ifndef VULKAN_SRC_ENGINE_PROPERTY_TRAITS_DEFAULT_ALWAYSVISIBLE_H_
#define VULKAN_SRC_ENGINE_PROPERTY_TRAITS_DEFAULT_ALWAYSVISIBLE_H_

#include "engine/property/traits/Visible.h"

class AlwaysVisible : public Visible {
 public:
  bool IsVisible() const override;
};

#endif  // VULKAN_SRC_ENGINE_PROPERTY_TRAITS_DEFAULT_ALWAYSVISIBLE_H_
