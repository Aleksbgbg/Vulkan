#ifndef VULKAN_SRC_ENGINE_UI_ELEMENT_H_
#define VULKAN_SRC_ENGINE_UI_ELEMENT_H_

#include <list>
#include <string>

struct Element {
  std::string title;
  class UiElement* content;
};

using ElementList = std::list<Element>;

#endif  // VULKAN_SRC_ENGINE_UI_ELEMENT_H_
