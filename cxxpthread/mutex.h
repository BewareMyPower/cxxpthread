#ifndef MUTEX_H
#define MUTEX_H

#include <pthread.h>
#include "error.h"
#include "noncopyable.h"

namespace cxxpthread {

class Mutex final : noncopyable {
 public:
  Mutex();
  ~Mutex();

  Mutex(const Mutex&) = delete;
  Mutex& operator=(const Mutex&) = delete;

  bool trylock() noexcept;
  void lock() noexcept;
  void unlock() noexcept;

  class Guard : noncopyable {
   public:
    Guard(Mutex& mutex) : mutex_(mutex) { mutex_.lock(); }
    ~Guard() { mutex_.unlock(); }

   private:
    Mutex& mutex_;
  };

 private:
  pthread_mutex_t handle_;
};

Mutex::Mutex() {
  int error = pthread_mutex_init(&handle_, nullptr);
  if (error != 0) handleError(error, "Mutex Constructor");
}

Mutex::~Mutex() { pthread_mutex_destroy(&handle_); }

inline bool Mutex::trylock() noexcept {
  const char* where = "Mutex::trylock()";
  int error = pthread_mutex_trylock(&handle_);
  if (error == 0) return true;
  if (error != EBUSY) handleError(error, "%s: pthread_mutex_trylock", where);
  return false;
}

inline void Mutex::lock() noexcept {
  const char* where = "Mutex::lock()";
  int error = pthread_mutex_lock(&handle_);
  if (error != 0) handleError(error, "%s: pthread_mutex_lock", where);
}

inline void Mutex::unlock() noexcept {
  const char* where = "Mutex::unlock()";
  int error = pthread_mutex_unlock(&handle_);
  if (error != 0) handleError(error, "%s: pthread_mutex_unlock", where);
}

}  // namespace cxxpthread

#endif  // MUTEX_H
