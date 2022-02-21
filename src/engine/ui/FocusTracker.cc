#include "FocusTracker.h"

#include "UiElement.h"

void BuildMapForNode(
    const UiTree::Node& node,
    std::unordered_map<const Focusable*, const UiTree::Node*>& map) {
  map.insert(std::make_pair(node.element.get(), &node));
  for (const std::unique_ptr<UiTree::Node>& child : node.children) {
    BuildMapForNode(*child, map);
  }
}

auto BuildFocusableToNodeMap(const UiTree& tree) {
  std::unordered_map<const Focusable*, const UiTree::Node*> map;
  BuildMapForNode(*tree.root, map);
  return map;
}

FocusTracker::FocusTracker(const UiTree& tree)
    : focusableToNode_(BuildFocusableToNodeMap(tree)), focused_(nullptr) {}

bool FocusTracker::HasFocusedElement() const {
  return focused_ != nullptr;
}

bool FocusTracker::HasFocus(const Focusable* focusable) const {
  return focusable == focused_;
}

bool FocusTracker::ChildHasFocus(const Focusable* focusable) const {
  return AreParentAndChild(focusable, focused_);
}

void FocusTracker::AcquireFocus(Focusable* focusable) {
  Focus(focusable);
}

void FocusTracker::RenounceFocus(Focusable* focusable) {
  const UiTree::Node* parent = NodeFor(focusable)->parent;

  if (parent == nullptr) {
    focused_->OnLostFocus();
    focused_ = nullptr;
    return;
  }

  Focus(parent->element.get());
}

const UiTree::Node* FocusTracker::NodeFor(const Focusable* focusable) const {
  return focusableToNode_.at(focusable);
}

bool FocusTracker::AreParentAndChild(const Focusable* parent,
                                     const Focusable* child) const {
  const UiTree::Node* expectedParent = NodeFor(parent);
  const UiTree::Node* current = NodeFor(child);

  while (current != nullptr) {
    if (current == expectedParent) {
      return true;
    }

    current = current->parent;
  }

  return false;
}

void FocusTracker::Focus(Focusable* focusable) {
  if (focused_ != nullptr) {
    focused_->OnLostFocus();
  }
  focused_ = focusable;
  focused_->OnAcquiredFocus();

  const UiTree::Node* current = NodeFor(focusable);

  while (current != nullptr) {
    current->element->OnChildAcquiredFocus();

    current = current->parent;
  }
}
