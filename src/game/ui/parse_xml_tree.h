#ifndef VULKAN_SRC_GAME_UI_PARSE_XML_TREE_H_
#define VULKAN_SRC_GAME_UI_PARSE_XML_TREE_H_

#include <optional>
#include <string_view>

#include "ParseTree.h"

namespace ui {

std::optional<ParseTree> ParseXmlTree(std::string_view content);

}  // namespace ui

#endif  // VULKAN_SRC_GAME_UI_PARSE_XML_TREE_H_
