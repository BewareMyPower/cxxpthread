// print_arg_type.h
#pragma once

#include <boost/type_index.hpp>
#include <iostream>

#define TYPE_NAME(T) boost::typeindex::type_id_with_cvr<T>().pretty_name()

template <size_t i>
inline void printArgsType(const char*, bool) {
  std::cout.flush();
}

template <size_t i, typename T, typename... Rest>
inline void printArgsType(const char* prefix, bool withindex = true) {
  std::cout << prefix;
  if (withindex) std::cout << "[" << i << "]";
  std::cout << " = " << TYPE_NAME(T) << "\n";
  printArgsType<i + 1, Rest...>(prefix, withindex);
}

template <typename T>
inline void printArgsType(const char* prefix) {
  printArgsType<0, T>(prefix, false);
}

#undef TYPE_NAME
