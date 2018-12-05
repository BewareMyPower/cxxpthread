// thread.h
#ifndef CXXPTHREAD_THREAD_H
#define CXXPTHREAD_THREAD_H

#include <pthread.h>
#include <memory>
#include "detail/thread_data.h"
#include "error.h"

namespace cxxpthread {

class Thread {
 public:
  Thread() = default;

  Thread(const Thread&) = delete;
  Thread& operator=(const Thread&) = delete;

  Thread(Thread&& rhs) noexcept : handle_(rhs.handle_), retval_(rhs.retval_) {
    rhs.reset();
  }

  Thread& operator=(Thread&& rhs) noexcept {
    if (handle_ == pthread_t{})
      handleError("Thread move assign: can't use a unjoined Thread as lvalue");
    handle_ = rhs.handle_;
    retval_ = rhs.retval_;
    rhs.reset();
    return *this;
  }

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
    int error = pthread_create(&handle_, nullptr, func, data);
    if (error != 0) handleError(error, "cxxpthread::Thread create failed");
  }

  void join() {
    int error = pthread_join(handle_, &retval_);
    if (error != 0) handleError(error, "cxxpthread::Thread::join()");
    handle_ = pthread_t{};
  }

  // If thread function return a value, getResult() will return an unique_ptr
  // object to that value.
  // NOTE: User must specify correct ResultType, or it may cause fatal error.
  template <typename ResultType>
  std::unique_ptr<ResultType> getResult() const {
    return std::unique_ptr<ResultType>(static_cast<ResultType*>(retval_));
  }

  static void Exit(long retval) {
    pthread_exit(reinterpret_cast<void*>(retval));
  }

  // If thread function use pthread_exit() to exit thread, getExitCode() will
  // return exit code.
  // If thread function return a value, getExitCode() will return a pointer to
  // that value.
  long getExitCode() const { return reinterpret_cast<long>(retval_); }

 private:
  pthread_t handle_{};
  void* retval_ = nullptr;

  void reset() noexcept {
    handle_ = pthread_t{};
    retval_ = nullptr;
  }
};

}  // namespace cxxpthread

#endif
