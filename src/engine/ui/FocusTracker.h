#ifndef SRC_ENGINE_UI_FOCUSTRACKER_H_
#define SRC_ENGINE_UI_FOCUSTRACKER_H_

#include <unordered_map>

#include "Focusable.h"
#include "UiTree.h"

class FocusTracker {
 public:
  explicit FocusTracker(const UiTree& tree);

  bool HasFocusedElement() const;
  bool HasFocus(const Focusable* focusable) const;
  bool ChildHasFocus(const Focusable* focusable) const;
  void AcquireFocus(Focusable* focusable);
  void RenounceFocus(Focusable* focusable);

 private:
  const UiTree::Node* NodeFor(const Focusable* focusable) const;
  bool AreParentAndChild(const Focusable* parent, const Focusable* child) const;
  void Focus(Focusable* focusable);

 private:
  std::unordered_map<const Focusable*, const UiTree::Node*> focusableToNode_;
  Focusable* focused_;
};

#endif  // SRC_ENGINE_UI_FOCUSTRACKER_H_
