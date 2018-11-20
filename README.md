# cxxpthread
(**TODO: It has some severe bugs to fix yet.**)
A C++11 wrapper of pthread library, inspired by c++11 `<thread>`.

`test.cc` is a simple test file, you can compile it by
```
g++ -std=c++11 -pthread test.cc
```
It's a header-only library, and only requires compiler which supports C++11. (eg. gcc 4.8)
However, if you want to enable macro `DEBUG`, Boost library is required.
