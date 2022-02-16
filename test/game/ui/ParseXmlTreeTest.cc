#include <gtest/gtest.h>

#include "game/ui/parse_xml_tree.h"

const ui::ParseTree::Node& NodeAtIndex(
    const std::list<ui::ParseTree::Node>& nodes, std::size_t index) {
  auto nodeIterator = nodes.begin();

  for (; index > 0; --index) {
    nodeIterator = std::next(nodeIterator);
  }

  return *nodeIterator;
}

const ui::ParseTree::Node& TraverseTree(
    const ui::ParseTree::Node& root,
    const std::initializer_list<std::size_t> indices) {
  const ui::ParseTree::Node* node = &root;

  for (const auto index : indices) {
    node = &NodeAtIndex(node->children, index);
  }

  return *node;
}

TEST(ParseXmlTreeTest, ParsesEmptyTree) {
  const std::string_view tree = "";

  const std::optional<ui::ParseTree> result = ui::ParseXmlTree(tree);

  ASSERT_EQ(std::nullopt, result);
}

TEST(ParseXmlTreeTest, ParsesRootNode) {
  const std::string_view tree = R"(<Root/>)";

  const std::optional<ui::ParseTree> result = ui::ParseXmlTree(tree);

  ASSERT_EQ("Root", result->root.name);
}

TEST(ParseXmlTreeTest, ParsesTagWithEmptyBody) {
  const std::string_view tree = R"(<Root></Root>)";

  const std::optional<ui::ParseTree> result = ui::ParseXmlTree(tree);

  ASSERT_EQ("Root", result->root.name);
}

TEST(ParseXmlTreeTest, ParsesSingleAttribute) {
  const std::string_view tree = R"(<Root AttributeA="ValueA"/>)";

  const std::optional<ui::ParseTree> result = ui::ParseXmlTree(tree);

  ASSERT_EQ("Root", result->root.name);
  ASSERT_EQ("ValueA", result->root.attributes.at("AttributeA"));
}

TEST(ParseXmlTreeTest, ParsesMultipleAttributes) {
  const std::string_view tree =
      R"(<Root AttributeA="ValueA" AttributeB="ValueB"/>)";

  const std::optional<ui::ParseTree> result = ui::ParseXmlTree(tree);

  ASSERT_EQ("ValueA", result->root.attributes.at("AttributeA"));
  ASSERT_EQ("ValueB", result->root.attributes.at("AttributeB"));
}

TEST(ParseXmlTreeTest, ParsesAttributesOnTagWithBody) {
  const std::string_view tree =
      R"(<Root AttributeA="ValueA" AttributeB="ValueB"></Root>)";

  const std::optional<ui::ParseTree> result = ui::ParseXmlTree(tree);

  ASSERT_EQ("ValueA", result->root.attributes.at("AttributeA"));
  ASSERT_EQ("ValueB", result->root.attributes.at("AttributeB"));
}

TEST(ParseXmlTreeTest, ParsesChildNodes) {
  const std::string_view tree = R"(<Root><Child1/><Child2/><Child3/></Root>)";

  const std::optional<ui::ParseTree> result = ui::ParseXmlTree(tree);

  ASSERT_EQ(3, result->root.children.size());
  ASSERT_EQ("Child1", TraverseTree(result->root, {0}).name);
  ASSERT_EQ("Child2", TraverseTree(result->root, {1}).name);
  ASSERT_EQ("Child3", TraverseTree(result->root, {2}).name);
}

TEST(ParseXmlTreeTest, ParsesBasicTreeStructure) {
  const std::string_view tree =
      R"(<Root><Child1><Grandchild1/></Child1><Child2><Grandchild2/></Child2></Root>)";

  const std::optional<ui::ParseTree> result = ui::ParseXmlTree(tree);

  ASSERT_EQ("Child1", TraverseTree(result->root, {0}).name);
  ASSERT_EQ("Grandchild1", TraverseTree(result->root, {0, 0}).name);
  ASSERT_EQ("Child2", TraverseTree(result->root, {1}).name);
  ASSERT_EQ("Grandchild2", TraverseTree(result->root, {1, 0}).name);
}

TEST(ParseXmlTreeTest, ParsesProperWhitespacing) {
  const std::string_view tree = R"(
<Root>
  <Child1>
  </Child1>
  <Child2>
    <GrandChild1>
      <GreatGrandChild1>
      </GreatGrandChild1>
      <GreatGrandChild2>
        <GreatGreatGrandChild1>
        </GreatGreatGrandChild1>
        <GreatGreatGrandChild2/>
        <GreatGreatGrandChild3></GreatGreatGrandChild3>
        <TargetNode>
        </TargetNode>
      </GreatGrandChild2>
    </GrandChild1>
  </Child2>
</Root>
)";

  const std::optional<ui::ParseTree> result = ui::ParseXmlTree(tree);

  ASSERT_EQ("TargetNode", TraverseTree(result->root, {1, 0, 1, 3}).name);
}

TEST(ParseXmlTreeTest, ParsesDeepAttributes) {
  const std::string_view tree = R"(
<Root>
  <Child1>
  </Child1>
  <Child2>
    <GrandChild1>
      <GreatGrandChild1>
      </GreatGrandChild1>
      <GreatGrandChild2>
        <GreatGreatGrandChild1>
        </GreatGreatGrandChild1>
        <GreatGreatGrandChild2/>
        <GreatGreatGrandChild3></GreatGreatGrandChild3>
        <TargetNode Key1="Value1" Key2="Value2">
        </TargetNode>
      </GreatGrandChild2>
    </GrandChild1>
  </Child2>
</Root>
)";

  const std::optional<ui::ParseTree> result = ui::ParseXmlTree(tree);

  ASSERT_EQ("Value2",
            TraverseTree(result->root, {1, 0, 1, 3}).attributes.at("Key2"));
}
