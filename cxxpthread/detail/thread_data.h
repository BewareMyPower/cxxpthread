// thread_base.h
#ifndef CXXPTHREAD_DETAIL_THREAD_DATA_H
#define CXXPTHREAD_DETAIL_THREAD_DATA_H

#include <pthread.h>
#include <memory>
#include <tuple>
#include <type_traits>
#include "index_tuple.h"
#include "invoke.h"

namespace cxxpthread {

namespace detail {

// Used by pthread entry function
struct BaseThreadData {
  virtual ~BaseThreadData() {}
  virtual void run() = 0;
};

template <typename F, typename... Args>
struct ThreadData : BaseThreadData {
  using TupleType = std::tuple<typename std::decay<F>::type,
                               typename std::decay<Args>::type...>;
  using FuncPtr = typename std::decay<F>::type;
  using ReturnType = typename std::result_of<FuncPtr(Args...)>::type;

  explicit ThreadData(F&& f, Args&&... args)
      : data_(std::forward<F>(f), std::forward<Args>(args)...) {}

  void run() override {
    using IndexTupleType = typename MakeIndexTuple<1, argsCount()>::type;
    RunImp<std::is_void<ReturnType>::value, TupleType, IndexTupleType>::run(
        data_);
  }

  template <bool retval_is_void, typename DataTuple, typename IndexTuple>
  struct RunImp;

  template <typename Tuple, std::size_t... indexes>
  struct RunImp<true, Tuple, IndexTuple<indexes...>> {
    static void run(Tuple& data) {
      invoke(std::move(std::get<0>(data)),
             std::move(std::get<indexes>(data))...);
    }
  };

  template <typename Tuple, std::size_t... indexes>
  struct RunImp<false, Tuple, IndexTuple<indexes...>> {
    static void run(Tuple& data) {
      auto&& ret = invoke(std::move(std::get<0>(data)),
                          std::move(std::get<indexes>(data))...);
      // FIXME: return value must have move constructor
      pthread_exit(new ReturnType(std::move(ret)));
    }
  };

  static constexpr std::size_t argsCount() {
    return std::tuple_size<TupleType>::value;
  }

  TupleType data_;
};

}  // namespace cxxpthread::detail

}  // namespace cxxpthread

#endif
