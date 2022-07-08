#ifndef SRC_ENGINE_PROPERTY_TYPE_GRAPHICALINTERFACE_H_
#define SRC_ENGINE_PROPERTY_TYPE_GRAPHICALINTERFACE_H_

#include "Visibility.h"
#include "engine/property/Property.h"
#include "engine/property/PropertyKey.h"

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

#endif  // SRC_ENGINE_PROPERTY_TYPE_GRAPHICALINTERFACE_H_
