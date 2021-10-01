#ifndef VULKAN_SRC_ITERABLE_H
#define VULKAN_SRC_ITERABLE_H

//template<typename Iter>
//auto MakeIterable(const Iter& value) {
//  return Iterable(value);
//}

//template<typename TIterator>
//class Iterable {
//  using value_type = typename std::iterator_traits<TIterator>::value_type;
//public:
//  template<typename Iterable>
//  explicit Iterable(const Iterable& iterable) : _begin(std::begin(iterable)), _end(std::end(iterable)) {}
//
//  auto begin() const {
//    return _begin;
//  }
//
//  auto end() const {
//    return _end;
//  }
//
//private:
//  TIterator _begin;
//  TIterator _end;
//};

#endif // VULKAN_SRC_ITERABLE_H
