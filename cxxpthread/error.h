// error.h
#ifndef CXXPTHREAD_ERROR_H
#define CXXPTHREAD_ERROR_H

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <utility>

namespace cxxpthread {

template <typename... Args>
inline void handleError(int error, const char* fmt, Args&&... args) {
  fprintf(stderr, fmt, std::forward<Args>(args)...);
  fprintf(stderr, ": %s\n", strerror(error));
  exit(1);
}

template <>
inline void handleError(int error, const char* msg) {
  fprintf(stderr, "%s: %s\n", msg, strerror(error));
  exit(1);
}

template <typename... Args>
inline void handleError(const char* fmt, Args&&... args) {
  handleError(errno, fmt, std::forward<Args>(args)...);
}

template <typename... Args>
inline void handleMessage(const char* fmt, Args&&... args) {
  fprintf(stderr, fmt, std::forward<Args>(args)...);
  fputs("\n", stderr);
  exit(1);
}

template <>
inline void handleMessage(const char* msg) {
  fprintf(stderr, "%s\n", msg);
  exit(1);
}

}  // namespace cxxpthread

#endif
