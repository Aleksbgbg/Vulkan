#include "parse_xml_tree.h"

namespace ui {

struct Attribute {
  std::string key;
  std::string value;
};

class StringScanner {
 public:
  StringScanner(const std::string_view content)
      : content_(content), position_(-1) {}

  bool IsEnd() const {
    return position_ >= content_.size();
  }

  char CurrentChar() const {
    return content_[position_];
  }

  void MoveForward() {
    ++position_;
  }

  bool CurrentCharIs(const std::initializer_list<char> values) const {
    const char currentChar = CurrentChar();

    for (const char value : values) {
      if (currentChar == value) {
        return true;
      }
    }

    return false;
  }

  bool MatchLookAhead(const std::string_view value) const {
    if ((position_ + value.size()) > content_.size()) {
      return false;
    }

    for (std::size_t position = 0; position < value.size(); ++position) {
      if (content_[position_ + position] != value[position]) {
        return false;
      }
    }

    return true;
  }

  void SkipUntil(const std::initializer_list<char> endValues) {
    do {
      MoveForward();
    } while (!IsEnd() && !CurrentCharIs(endValues));
  }

  std::string BufferCharactersUntil(
      const std::initializer_list<char> endValues) {
    std::string buffer;

    while (true) {
      MoveForward();

      if (IsEnd() || CurrentCharIs(endValues)) {
        break;
      }

      buffer.push_back(CurrentChar());
    }

    return buffer;
  }

 private:
  const std::string_view content_;
  std::size_t position_;
};

class AttributeParser {
 public:
  AttributeParser(StringScanner& scanner) : scanner_(scanner), attribute_() {}

  std::optional<Attribute> ParseAttribute() {
    attribute_.key = scanner_.BufferCharactersUntil({'='});
    scanner_.MoveForward();
    attribute_.value = scanner_.BufferCharactersUntil({'"'});
    return attribute_;
  }

 private:
  StringScanner& scanner_;
  Attribute attribute_;
};

class NodeParser {
 public:
  NodeParser(StringScanner& scanner) : scanner_(scanner), node_() {}

  std::optional<ParseTree::Node> ParseNode() {
    scanner_.SkipUntil({'<'});

    if (scanner_.IsEnd()) {
      return std::nullopt;
    }

    node_.name = scanner_.BufferCharactersUntil({' ', '>', '/'});

    if (scanner_.CurrentChar() == ' ') {
      ParseAttributes();

      if (scanner_.CurrentChar() == '>') {
        ParseChildren();
      }
    } else if (scanner_.CurrentChar() == '>') {
      ParseChildren();
    } else if (!RestOfTagEmpty()) {
      return std::nullopt;
    }

    return node_;
  }

 private:
  void ParseAttributes() {
    while (true) {
      std::optional<Attribute> attribute =
          AttributeParser(scanner_).ParseAttribute();

      if (!attribute.has_value()) {
        break;
      }

      node_.attributes.insert(std::make_pair(std::move(attribute->key),
                                             std::move(attribute->value)));

      scanner_.MoveForward();

      if (scanner_.CurrentCharIs({'>', '/'})) {
        break;
      }
    }
  }

  void ParseChildren() {
    while (true) {
      std::optional<ParseTree::Node> child = NodeParser(scanner_).ParseNode();

      if (!child.has_value()) {
        break;
      }

      node_.children.push_back(std::move(child.value()));
    }
  }

  bool RestOfTagEmpty() const {
    return ShorthandCloseTagAhead() || ExpandedEmptyTagAhead();
  }

  bool ShorthandCloseTagAhead() const {
    return scanner_.MatchLookAhead("/>");
  }

  bool ExpandedEmptyTagAhead() const {
    return scanner_.MatchLookAhead(std::string("></") + node_.name + ">");
  }

 private:
  StringScanner& scanner_;
  ParseTree::Node node_;
};

class Parser {
 public:
  Parser(StringScanner scanner) : scanner_(scanner) {}

  std::optional<ParseTree> Parse() {
    std::optional<ParseTree::Node> root = NodeParser(scanner_).ParseNode();

    if (root.has_value()) {
      ParseTree tree;
      tree.root = std::move(root.value());
      return tree;
    }

    return std::nullopt;
  }

 private:
  StringScanner scanner_;
};

std::optional<ParseTree> ParseXmlTree(const std::string_view content) {
  return Parser(StringScanner(content)).Parse();
}

}  // namespace ui
