# cxxpthread
A C++11 wrapper of pthread library, inspired by c++11 `<thread>`.

It's a header-only library, and only requires compiler which supports C++11. (eg. gcc 4.8)
However, if you want to enable macro `DEBUG`, Boost library is required.

**TODO: It doesn't support function object yet.**
see [function_object_test.cc](./test/function_object_test.cc)
