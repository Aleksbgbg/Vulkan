#ifndef SRC_ENGINE_UI_UITREE_H_
#define SRC_ENGINE_UI_UITREE_H_

#include <memory>
#include <vector>

class UiElement;

struct UiTree {
  struct Node {
    Node* parent;
    std::unique_ptr<UiElement> element;
    std::vector<std::unique_ptr<Node>> children;
  };

  std::unique_ptr<Node> root;
};

#endif  // SRC_ENGINE_UI_UITREE_H_
