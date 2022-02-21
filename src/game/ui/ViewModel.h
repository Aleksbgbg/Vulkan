#ifndef VULKAN_SRC_GAME_UI_VIEWMODEL_H_
#define VULKAN_SRC_GAME_UI_VIEWMODEL_H_

#include <functional>
#include <string>
#include <string_view>

#include "Binding.h"
#include "Bound.h"
#include "core/adapters/function_traits.h"
#include "game/actor/resource/Resource.h"

#define BIND(NAME)                                                       \
  Bind<NthArgType<                                                       \
      0U, decltype(&std::remove_pointer_t<decltype(this)>::Set##NAME)>>( \
      #NAME, [this](const auto value) { Set##NAME(value); })

class ViewModel : public Resource {
 public:
  template <typename T>
  Bound<T>& GetBinding(const std::string& key) const {
    return reinterpret_cast<Bound<T>&>(*bindings_.at(key));
  }

 protected:
  template <typename T>
  Bound<T>& Bind(const std::string_view variable,
                 const std::function<void(T)> set) {
    std::unique_ptr<Bound<T>> binding = std::make_unique<Bound<T>>(set);
    Bound<T>& boundValue = *binding;
    bindings_.insert(std::make_pair(variable, std::move(binding)));
    return boundValue;
  }

 private:
  std::unordered_map<std::string, std::unique_ptr<ui::Binding>> bindings_;
};

#endif  // VULKAN_SRC_GAME_UI_VIEWMODEL_H_
