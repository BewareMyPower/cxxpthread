// function_object_test.cc
#include "thread.h"
using namespace cxxpthread;

#include <functional>  // std::ref
#include <iostream>    // std::cout
using namespace std;

struct Object {
  void operator()() const { cout << "Object()" << endl; }
};

int main() {
  // FIXME: compile error
  //{
  //  Object obj;
  //  Thread t{obj};
  //  t.join();
  //}
  cerr << "TODO: Thread(F&&, Args&&...) [F = Object&; Args={}]" << endl;
  return 0;
}
/* error message:
In file included from function_object_test.cc:2:0:
../cxxpthread/thread.h: In instantiation of ‘cxxpthread::Thread::Thread(F&&, Args&& ...) [with F = Object&; Args = {}]’:
function_object_test.cc:16:17:   required from here
../cxxpthread/thread.h:32:72: error: cannot bind ‘Object’ lvalue to ‘Object&&’
         new DerivedType(std::forward<F>(f), std::forward<Args>(args)...);
                                                                        ^
In file included from ../cxxpthread/thread.h:7:0,
                 from function_object_test.cc:2:
../cxxpthread/detail/thread_data.h:29:12: note:   initializing argument 1 of ‘cxxpthread::detail::ThreadData<F, Args>::ThreadData(F&&, Args&& ...) [with F = Object; Args = {}]’
   explicit ThreadData(F&& f, Args&&... args)
*/
