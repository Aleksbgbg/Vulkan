#ifndef SRC_ENGINE_UI_ELEMENT_H_
#define SRC_ENGINE_UI_ELEMENT_H_

#include <list>
#include <string>

struct Element {
  std::string title;
  class UiElement* content;
};

using ElementList = std::list<Element>;

#endif  // SRC_ENGINE_UI_ELEMENT_H_
