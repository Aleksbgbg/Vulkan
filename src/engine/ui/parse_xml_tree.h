#ifndef SRC_ENGINE_UI_PARSE_XML_TREE_H_
#define SRC_ENGINE_UI_PARSE_XML_TREE_H_

#include <optional>
#include <string_view>

#include "ParseTree.h"

namespace ui {

std::optional<ParseTree> ParseXmlTree(std::string_view content);

}  // namespace ui

#endif  // SRC_ENGINE_UI_PARSE_XML_TREE_H_
