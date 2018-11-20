// invoke.h
#ifndef CXXPTHREAD_DETAIL_INVOKE_H
#define CXXPTHREAD_DETAIL_INVOKE_H

#include <functional>
#include <utility>

namespace cxxpthread {

namespace detail {

struct NormalFunc {};
struct MemberFunc {};

template <typename F, bool is_member_func>
struct FuncTraits;

template <typename F>
struct FuncTraits<F, false> {
  using type = NormalFunc;
};

template <typename F>
struct FuncTraits<F, true> {
  using type = MemberFunc;
};

template <typename F, typename... Args>
auto invokeImp(NormalFunc, F&& f, Args&&... args) ->
    typename std::result_of<F(Args...)>::type {
  return std::forward<F>(f)(std::forward<Args>(args)...);
}

template <typename F, typename... Args>
auto invokeImp(MemberFunc, F&& f, Args&&... args) ->
    typename std::result_of<F(Args...)>::type {
  return std::mem_fn(std::forward<F>(f))(std::forward<Args>(args)...);
}

// Examples:
// 1. invoke normal function
//   int f(char);
//   int ret = invoke(f, 'x');
// 2. invoke class member function
//   struct Object {
//     const char* f(int, const char*);
//   };
//   Object obj;
//   auto ret = invoke(&Object::f, &obj, 10, "hello");
// 3. invoke function object
//   struct Object {
//     void operator()(double);
//   };
//   invoke(Object(), 3.14);
// 4. invoke lambda expression
//   invoke([](int i) { printf("%d\n", i); }, 10);
//   invoke([&obj] { obj.f(10, "hello"); });
template <typename F, typename... Args>
auto invoke(F&& f, Args&&... args) ->
    typename std::result_of<F(Args...)>::type {
  using FuncType =
      typename FuncTraits<F, std::is_member_function_pointer<F>::value>::type;

  return invokeImp(FuncType{}, std::forward<F>(f), std::forward<Args>(args)...);
}

}  // namespace cxxpthread::detail

}  // namespace cxxpthread

#endif
