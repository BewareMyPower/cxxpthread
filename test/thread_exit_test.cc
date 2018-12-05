// thread_exit_test.cc
#include "thread.h"
using namespace cxxpthread;

#include <iostream>  // std::cout
using namespace std;

int main() {
  int x = 3, y = 4;
  {
    Thread t{[](int x, int y) { return x + y; }, x, y};
    t.join();
    // user must specify correct return type, or it will cause memory leak.
    auto p = t.getResult<int>();
    cout << x << " + " << y << " = " << *p << endl;
    cout << "Thread exit with " << reinterpret_cast<void*>(t.getExitCode())
         << endl;
  }
  {
    Thread t{[] { pthread_exit(reinterpret_cast<void*>(1L)); }};
    t.join();
    cout << "Thread exit with " << t.getExitCode() << endl;
  }
  {
    Thread t{[] {}};
    t.join();
    cout << "Thread exit with " << t.getExitCode() << endl;
  }
  return 0;
}
/*
3 + 4 = 7
Thread exit with 0x7fa32c0008c0
Thread exit with 1
Thread exit with 0
*/
