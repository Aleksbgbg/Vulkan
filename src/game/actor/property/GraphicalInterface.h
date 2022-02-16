#ifndef VULKAN_SRC_GAME_ACTOR_PROPERTY_GRAPHICALINTERFACE_H_
#define VULKAN_SRC_GAME_ACTOR_PROPERTY_GRAPHICALINTERFACE_H_

#include "Property.h"
#include "PropertyKey.h"
#include "Visibility.h"

class GraphicalInterface : public Property {
 public:
  static constexpr PropertyKey Key() {
    return PropertyKey::GraphicalInterface;
  }

  GraphicalInterface(Visibility& visibility);

  bool IsVisible() const;

  void Show();
  void Hide();

  // A way for the interface container to request that scripts hide the
  // interface, otherwise the two contradict each other because they process the
  // same UI events.
  bool HideRequested() const;
  void RequestHide();

 private:
  Visibility& visibility_;
  bool hideRequested_;
};

#endif  // VULKAN_SRC_GAME_ACTOR_PROPERTY_GRAPHICALINTERFACE_H_
