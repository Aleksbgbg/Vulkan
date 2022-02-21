#ifndef VULKAN_SRC_CORE_ADAPTERS_FUNCTION_TRAITS_H_
#define VULKAN_SRC_CORE_ADAPTERS_FUNCTION_TRAITS_H_

template <std::size_t N, typename TArg0, typename... TArgs>
struct NthType {
  using Type = typename NthType<N - 1U, TArgs...>::Type;
};

template <typename TArg0, typename... TArgs>
struct NthType<0U, TArg0, TArgs...> {
  using Type = TArg0;
};

template <std::size_t, typename>
struct NthArg;

template <std::size_t N, typename TReturn, typename... TArgs>
struct NthArg<N, TReturn(TArgs...)> {
  using Type = typename NthType<N, TArgs...>::Type;
};

template <std::size_t N, typename TClass, typename TReturn, typename... TArgs>
struct NthArg<N, TReturn (TClass::*)(TArgs...)> {
  using Type = typename NthType<N, TArgs...>::Type;
};

template <std::size_t N, typename TClass, typename TReturn, typename... TArgs>
struct NthArg<N, TReturn (TClass::*)(TArgs...) const> {
  using Type = typename NthType<N, TArgs...>::Type;
};

template <std::size_t N, typename TFunc>
struct NthArg : public NthArg<N, decltype(&TFunc::operator())> {};

template <std::size_t N, typename TFunction>
using NthArgType = typename NthArg<N, TFunction>::Type;

#endif  // VULKAN_SRC_CORE_ADAPTERS_FUNCTION_TRAITS_H_
