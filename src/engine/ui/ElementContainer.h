#ifndef VULKAN_SRC_ENGINE_UI_ELEMENTCONTAINER_H_
#define VULKAN_SRC_ENGINE_UI_ELEMENTCONTAINER_H_

#include <vector>

#include "Bindable.h"
#include "Element.h"
#include "SelectionState.h"
#include "core/math/wrap.h"

template <typename T>
class ContainerElement;

template <typename T>
class ElementContainer;

template <typename T>
class ElementActivator {
 public:
  virtual ~ElementActivator() = default;

  virtual void Select(ContainerElement<T>& element) = 0;
  virtual void Hover(ContainerElement<T>& element) = 0;
};

template <typename T>
class ContainerElement {
  friend ElementContainer<T>;

 public:
  const T* operator->() const {
    return &info_;
  }

  T* operator->() {
    return &info_;
  }

  bool IsSelected() const {
    return selectionState_ == SelectionState::Selected;
  }

  SelectionState Selection() const {
    return selectionState_;
  }

  std::string_view Title() const {
    return element_.title;
  }

  const UiElement& GetUiElement() const {
    return *element_.content;
  }

  UiElement& GetUiElement() {
    return *element_.content;
  }

  void Select() {
    activator_->Select(*this);
  }

  void Hover() {
    activator_->Hover(*this);
  }

 private:
  ElementActivator<T>* activator_;
  u32 index_;
  SelectionState selectionState_;
  Element element_;
  T info_;
};

template <typename T>
class ElementContainer : private ElementActivator<T> {
 private:
  std::vector<ContainerElement<T>> elements_;
  Bindable<u32> selectedIndex_;
  ContainerElement<T>* hoveredElement_;

  using ElementIterator = typename decltype(elements_)::iterator;
  using ConstElementIterator = typename decltype(elements_)::const_iterator;

 public:
  template <typename TIterator>
  class ElementReferenceIterator {
   public:
    constexpr explicit ElementReferenceIterator(const TIterator iterator)
        : iterator_(iterator) {}

    constexpr bool operator!=(const ElementReferenceIterator& other) const {
      return iterator_ != other.iterator_;
    }

    constexpr ElementReferenceIterator& operator++() {
      ++iterator_;
      return *this;
    }

    constexpr auto& operator*() const {
      return *iterator_;
    }

   private:
    TIterator iterator_;
  };

  explicit ElementContainer(ElementList& elements)
      : elements_(elements.size()),
        selectedIndex_(0),
        hoveredElement_(nullptr) {
    auto element = std::make_move_iterator(std::begin(elements));
    for (std::size_t index = 0; index < elements_.size(); ++index) {
      ContainerElement<T>& containerElement = elements_.at(index);
      containerElement.activator_ = this;
      containerElement.index_ = index;
      containerElement.element_ = *element;

      element = std::next(element);
    }

    elements_[0].selectionState_ = SelectionState::Selected;
  }

  ElementContainer(ElementList& elements, Bindable<u32> selected)
      : elements_(elements.size()),
        selectedIndex_(std::move(selected)),
        hoveredElement_(nullptr) {
    auto element = std::make_move_iterator(std::begin(elements));
    for (std::size_t index = 0; index < elements_.size(); ++index) {
      ContainerElement<T>& containerElement = elements_.at(index);
      containerElement.activator_ = this;
      containerElement.index_ = index;
      containerElement.element_ = *element;

      element = std::next(element);
    }

    elements_[0].selectionState_ = SelectionState::Selected;
  }

  constexpr auto begin() const {
    return ElementReferenceIterator<ConstElementIterator>(
        std::begin(elements_));
  }
  constexpr auto end() const {
    return ElementReferenceIterator<ConstElementIterator>(std::end(elements_));
  }

  constexpr auto begin() {
    return ElementReferenceIterator<ElementIterator>(std::begin(elements_));
  }
  constexpr auto end() {
    return ElementReferenceIterator<ElementIterator>(std::end(elements_));
  }

  std::size_t size() const {
    return elements_.size();
  }

  const ContainerElement<T>& at(const std::size_t index) const {
    return elements_.at(index);
  }
  ContainerElement<T>& at(const std::size_t index) {
    return elements_.at(index);
  }

  u32 SelectedIndex() const {
    return selectedIndex_;
  }

  const ContainerElement<T>& SelectedElement() const {
    return elements_[selectedIndex_];
  }
  ContainerElement<T>& SelectedElement() {
    return elements_[selectedIndex_];
  }
  const ContainerElement<T>* HoveredElement() const {
    return hoveredElement_;
  }

  const UiElement& ActiveUiElement() const {
    return *SelectedElement().element_.content;
  }
  UiElement& ActiveUiElement() {
    return *SelectedElement().element_.content;
  }

  void SelectNext() {
    SelectIndex(SelectedElement().index_ + 1);
  }
  void SelectPrevious() {
    SelectIndex(SelectedElement().index_ - 1);
  }

  void ClearHover() {
    if (hoveredElement_ == nullptr) {
      return;
    }

    if (hoveredElement_->selectionState_ != SelectionState::Selected) {
      hoveredElement_->selectionState_ = SelectionState::Inactive;
    }
    hoveredElement_ = nullptr;
  }

 private:
  void Select(ContainerElement<T>& element) override {
    SelectedElement().selectionState_ = SelectionState::Inactive;
    selectedIndex_ = element.index_;
    SelectedElement().selectionState_ = SelectionState::Selected;
  }
  void Hover(ContainerElement<T>& element) override {
    if (hoveredElement_ != nullptr) {
      if (hoveredElement_->selectionState_ == SelectionState::Selected) {
        return;
      }

      hoveredElement_->selectionState_ = SelectionState::Inactive;
    }
    hoveredElement_ = &element;
    if (hoveredElement_->selectionState_ != SelectionState::Selected) {
      hoveredElement_->selectionState_ = SelectionState::Hovered;
    }
  }

  void SelectIndex(i32 index) {
    Select(elements_[Wrap<i32>(index, 0, elements_.size() - 1)]);
  }
};

#endif  // VULKAN_SRC_ENGINE_UI_ELEMENTCONTAINER_H_
