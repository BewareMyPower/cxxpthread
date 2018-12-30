// thread.h
#ifndef CXXPTHREAD_THREAD_H
#define CXXPTHREAD_THREAD_H

#include <assert.h>
#include <pthread.h>
#include <memory>
#include "detail/thread_data.h"
#include "error.h"

namespace cxxpthread {

class Thread final {
 public:
  Thread() = default;

  Thread(const Thread&) = delete;
  Thread& operator=(const Thread&) = delete;

  Thread(Thread&& rhs) noexcept;
  Thread& operator=(Thread&& rhs) noexcept;

  // Start a thread function f(args...), if failed, print error and exit.
  template <typename F, typename... Args>
  Thread(F&& f, Args&&... args) noexcept;

  // Join a thread, If failed, print error and exit.
  void join() noexcept;

  // If thread function return a value, it innerlly call pthread_exit(), which
  // parameter is a pointer to a memory allocated by new. To avoid memory leak,
  // you must call getResult() to hold the pointer and automatically release it.
  template <typename T>
  std::unique_ptr<T> getResult() const {
    return std::unique_ptr<T>(static_cast<T*>(ret_));
  }

  // If thread function call pthread_join() to report an integral error code,
  // getResult() will delete this error code and cause segment fault.
  // To check the error code, you should call getExitCode().
  long getExitCode() const noexcept { return reinterpret_cast<long>(ret_); }

  static void exit(long error) {  // NOTE: noexcept will cause exception here
    pthread_exit(reinterpret_cast<void*>(error));
  }

 private:
  pthread_t handle_;
  void* ret_ = nullptr;
};

// implement of Thread member

Thread::Thread(Thread&& rhs) noexcept : handle_(rhs.handle_), ret_(rhs.ret_) {
  rhs.handle_ = pthread_t{};
  rhs.ret_ = nullptr;
}

Thread& Thread::operator=(Thread&& rhs) noexcept {
  assert(this != &rhs);
  handle_ = rhs.handle_;
  ret_ = rhs.ret_;
  rhs.handle_ = pthread_t{};
  rhs.ret_ = nullptr;
  return *this;
}

template <typename F, typename... Args>
Thread::Thread(F&& f, Args&&... args) noexcept {
  using BaseType = detail::BaseThreadData;
  using DerivedType = detail::ThreadData<F, Args...>;

  auto func = [](void* data) -> void* {
    std::unique_ptr<BaseType> p(static_cast<BaseType*>(data));
    p->run();
    return nullptr;
  };

  auto data = new DerivedType(std::forward<F>(f), std::forward<Args>(args)...);
  int error = pthread_create(&handle_, nullptr, func, data);
  if (error != 0) handleError(error, "cxxpthread::Thread create failed");
}

void Thread::join() noexcept {
  int error = pthread_join(handle_, &ret_);
  if (error != 0) handleError(error, "cxxpthread::Thread::join()");
  handle_ = pthread_t{};
}

}  // namespace cxxpthread

#endif
