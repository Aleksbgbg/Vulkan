#ifndef VULKAN_SRC_GAME_UI_PARSETREE_H_
#define VULKAN_SRC_GAME_UI_PARSETREE_H_

#include <list>
#include <string>
#include <unordered_map>

namespace ui {

struct ParseTree {
  struct Node {
    std::string name;
    std::unordered_map<std::string, std::string> attributes;
    std::list<Node> children;
  };

  Node root;
};

}  // namespace ui

#endif  // VULKAN_SRC_GAME_UI_PARSETREE_H_
