#ifndef SRC_ENGINE_PROPERTY_PROPERTYCOLLECTION_H_
#define SRC_ENGINE_PROPERTY_PROPERTYCOLLECTION_H_

#include <memory>
#include <unordered_map>

#include "Property.h"
#include "PropertyKey.h"

namespace game {

class PropertyCollection {
 public:
  template <typename TProperty, typename... TArgs>
  void EmplaceProperty(TArgs&&... args) {
    properties_.insert(
        {TProperty::Key(), std::make_unique<TProperty>(args...)});
  }

  template <typename T>
  const T& RetrieveProperty() const {
    return static_cast<const T&>(*properties_.at(T::Key()));
  }

  template <typename T>
  T& RetrieveProperty() {
    return static_cast<T&>(*properties_.at(T::Key()));
  }

 private:
  std::unordered_map<PropertyKey, std::unique_ptr<Property>> properties_;
};

}  // namespace game

#endif  // SRC_ENGINE_PROPERTY_PROPERTYCOLLECTION_H_
