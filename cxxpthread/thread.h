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

  // Unjoined thread will be joined in destructor.
  ~Thread() noexcept;

  // Join a thread, If failed, print error and exit.
  void join() noexcept;

  bool hasJoined() const noexcept { return has_joined_; }

  // If thread function return a value, it innerlly call pthread_exit(), which
  // parameter is a pointer to a memory allocated by new. To avoid memory leak,
  // you must call getResult() to hold the pointer and automatically release it.
  template <typename T>
  std::unique_ptr<T> getResult() const;

  // If thread function call pthread_join() to report an integral error code,
  // getResult() will delete this error code and cause segment fault.
  // To check the error code, you should call getExitCode().
  long getExitCode() const noexcept;

  // NOTE: noexcept will cause exception here because in C++, pthread_exit()
  // throws abi::__forced_unwind exception.
  static void exit(long error) { pthread_exit(reinterpret_cast<void*>(error)); }

 private:
  pthread_t handle_;
  void* ret_ = nullptr;
  bool has_joined_ = true;

  void init() noexcept;
};

// implement of Thread member

Thread::Thread(Thread&& rhs) noexcept : handle_(rhs.handle_),
                                        ret_(rhs.ret_),
                                        has_joined_(rhs.has_joined_) {
  rhs.init();
}

Thread& Thread::operator=(Thread&& rhs) noexcept {
  assert(this != &rhs);
  handle_ = rhs.handle_;
  ret_ = rhs.ret_;
  has_joined_ = rhs.has_joined_;

  rhs.init();
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

  has_joined_ = false;
}

Thread::~Thread() {
  if (!hasJoined()) join();
}

inline void Thread::join() noexcept {
  const char* where = "cxxpthread::Thread::join()";
  if (hasJoined()) handleMessage("%s: Already has joined.", where);

  int error = pthread_join(handle_, &ret_);
  if (error != 0) handleError(error, "%s: pthread_join failed", where);

  has_joined_ = true;
}

template <typename T>
inline std::unique_ptr<T> Thread::getResult() const {
  const char* where = "cxxpthread::Thread::getResult<T>";
  if (!hasJoined()) handleMessage("%s: Already has joined.", where);

  return std::unique_ptr<T>(static_cast<T*>(ret_));
}

long Thread::getExitCode() const noexcept {
  static_assert(sizeof(long) == sizeof(void*),
                "getExitCode(): convert void* to long");
  const char* where = "cxxpthread::Thread::getExitCode()";
  if (!hasJoined()) handleMessage("%s: Already has joined.", where);

  return reinterpret_cast<long>(ret_);
}

inline void Thread::init() noexcept {
  handle_ = pthread_t();
  ret_ = nullptr;
  has_joined_ = true;
}

}  // namespace cxxpthread

#endif
