// thread.h
#ifndef CXXPTHREAD_THREAD_H
#define CXXPTHREAD_THREAD_H

#include <pthread.h>
#include <memory>
#include "detail/thread_data.h"
#include "error.h"

#ifdef DEBUG
#include "print_arg_type.h"
#endif

namespace cxxpthread {

class Thread {
 public:
  // Start a thread function f(args...), if failed, print error and exit.
  template <typename F, typename... Args>
  Thread(F&& f, Args&&... args) noexcept {
    using BaseType = detail::BaseThreadData;
    using DerivedType = detail::ThreadData<F, Args...>;

    auto func = [](void* data) -> void* {
      std::unique_ptr<BaseType> p(static_cast<BaseType*>(data));
      p->run();
      return nullptr;
    };

    auto data =
        new DerivedType(std::forward<F>(f), std::forward<Args>(args)...);
#ifdef DEBUG
    printArgsType<F>("F");
    printArgsType<1, Args...>("Args");
#endif
    int error = pthread_create(&handle_, nullptr, func, data);
    if (error != 0) handleError(error, "cxxpthread::Thread create failed");
  }

  template <typename T = int>
  std::unique_ptr<T> join() {
    void* data;
    int error = pthread_join(handle_, &data);
    if (error != 0) handleError(error, "cxxpthread::Thread::join()");

    return std::unique_ptr<T>(static_cast<T*>(data));
  }

 private:
  pthread_t handle_;
};

}  // namespace cxxpthread

#endif
