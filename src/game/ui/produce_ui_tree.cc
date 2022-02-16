#include "produce_ui_tree.h"

#include "Orientation.h"
#include "TextCase.h"
#include "game/ui/elements/gridlist/GridListUiElement.h"
#include "game/ui/elements/listbox/ListBoxUiElement.h"
#include "game/ui/elements/slider/SliderUiElement.h"
#include "game/ui/elements/togglebox/ToggleBoxUiElement.h"
#include "general/strings/split.h"
#include "util/types.h"

template <typename TEnum>
TEnum ParseEnum(const std::string& value);

#define BEGIN_ENUM_PARSER(ENUM)              \
  template <>                                \
  ENUM ParseEnum(const std::string& value) { \
    static std::unordered_map<std::string, ENUM> parseValues {
#define ENUM_STRING(STRING, ENUM) {(STRING), (ENUM)},
#define END_ENUM_PARSER      \
  }                          \
  ;                          \
  return parseValues[value]; \
  }

BEGIN_ENUM_PARSER(Orientation)
ENUM_STRING("Horizontal", Orientation::Horizontal)
ENUM_STRING("Vertical", Orientation::Vertical)
END_ENUM_PARSER

BEGIN_ENUM_PARSER(TextCase)
ENUM_STRING("Uppercase", TextCase::Uppercase)
END_ENUM_PARSER

float ParseFloat(const std::string_view value) {
  return std::stof(value.data());
}

Margin ParseMargin(const std::string_view value) {
  std::vector<std::string> split = Split(value, ",");

  if (split.size() == 1) {
    const std::string& marginString = split[0];

    Margin margin;

    if (marginString.ends_with('*')) {
      const float marginFraction =
          ParseFloat(marginString.substr(0, marginString.size() - 1));

      margin.unit = DistanceUnit::Fractional;
      margin.fractional = Rectf::FromPoints(marginFraction, marginFraction,
                                            marginFraction, marginFraction);
    } else {
      throw std::runtime_error("Pixel distance not yet supported.");
    }

    return margin;
  } else {
    throw std::runtime_error(
        std::string("Margin had an invalid amount of components (") +
        std::to_string(split.size()) + ")");
  }
}

class UiNodeParser {
 public:
  UiNodeParser(const ui::ParseTree::Node& node, const ViewModel& viewModel)
      : parseNode_(node),
        uiNode_(std::make_unique<UiTree::Node>()),
        viewModel_(viewModel) {}

  std::unique_ptr<UiTree::Node> Parse() {
    if (parseNode_.name == "ListBox") {
      ListBoxParse parse = ParseListBox();
      uiNode_->element = std::make_unique<ListBoxUiElement>(parse);
    } else if (parseNode_.name == "GridList") {
      GridListParse parse = ParseGridList();
      uiNode_->element = std::make_unique<GridListUiElement>(parse);
    } else if (parseNode_.name == "Slider") {
      SliderParse parse = ParseSlider();
      uiNode_->element = std::make_unique<SliderUiElement>(parse);
    } else if (parseNode_.name == "ToggleBox") {
      ToggleBoxParse parse = ParseToggleBox();
      uiNode_->element = std::make_unique<ToggleBoxUiElement>(parse);
    }

    return std::move(uiNode_);
  }

 private:
  ListBoxParse ParseListBox() {
    return {.orientation = ParseEnumAttribute<Orientation>("Orientation"),
            .textCase = ParseEnumAttribute<TextCase>("TextCase"),
            .elements = ParseElements(),
            .margin = ParseMargin(AttributeOrDefault("Margin", "0.0*"))};
  }

  GridListParse ParseGridList() {
    return {.elements = ParseElements()};
  }

  SliderParse ParseSlider() const {
    return {.defaultValue = ParseFloat(parseNode_.attributes.at("Default")),
            .valueBinding = Bind<float>(parseNode_.attributes.at("Value"))};
  }

  ToggleBoxParse ParseToggleBox() {
    return {.elements = ParseElementsNoBody(),
            .selectedIndexBinder =
                Bind<u32>(parseNode_.attributes.at("SelectedIndex"))};
  }

  template <typename TEnum>
  TEnum ParseEnumAttribute(const std::string_view name) const {
    if (parseNode_.attributes.contains(name.data())) {
      return ParseEnum<TEnum>(parseNode_.attributes.at(name.data()));
    }

    return static_cast<TEnum>(0);
  }

  ElementList ParseElements() {
    ElementList elements;

    for (const ui::ParseTree::Node& child : parseNode_.children) {
      std::unique_ptr<UiTree::Node> node =
          UiNodeParser(child.children.front(), viewModel_).Parse();
      node->parent = uiNode_.get();
      elements.push_back({
          .title = child.attributes.at("Title"),
          .content = node->element.get(),
      });
      uiNode_->children.push_back(std::move(node));
    }

    return elements;
  }

  ElementList ParseElementsNoBody() const {
    ElementList elements;

    for (const ui::ParseTree::Node& child : parseNode_.children) {
      elements.push_back({.title = child.attributes.at("Title")});
    }

    return elements;
  }

  std::string AttributeOrDefault(const std::string_view attribute,
                                 const std::string_view defaultValue) const {
    if (parseNode_.attributes.contains(attribute.data())) {
      return parseNode_.attributes.at(attribute.data());
    }

    return std::string(defaultValue);
  }

  template <typename T>
  Bound<T>& Bind(const std::string& value) const {
    assert(value.starts_with("{Binding "));
    assert(value.ends_with("}"));

    static constexpr std::size_t IgnoredCharacters = sizeof("{Binding");
    const std::string key =
        value.substr(IgnoredCharacters, value.size() - IgnoredCharacters - 1);
    return viewModel_.GetBinding<T>(key);
  }

 private:
  const ui::ParseTree::Node& parseNode_;
  const ViewModel& viewModel_;
  std::unique_ptr<UiTree::Node> uiNode_;
};

std::optional<UiTree> ProduceUiTree(
    const std::optional<ui::ParseTree>& parseTree, const ViewModel& viewModel) {
  if (!parseTree.has_value()) {
    return std::nullopt;
  }

  UiTree tree;
  tree.root = UiNodeParser(parseTree->root, viewModel).Parse();
  return tree;
}
