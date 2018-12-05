// c_function_test.cc
#include "thread.h"
using namespace cxxpthread;

#include <functional>  // std::ref
#include <iostream>    // std::cout
using namespace std;

struct Object {
  void f(int i) const { cout << "Object::f(" << i << ")" << endl; }
};

int main() {
  // function without parameters
  {
    Thread t{[] { cout << "f()" << endl; }};
    t.join();
  }
  // function with parameters
  {
    Thread t{
        [](int i, double d) { cout << "f(" << i << ", " << d << ")" << endl; },
        1, 3.14};
    t.join();
  }
  // function with reference paramater
  {
    int i = 100;
    Object object;
    Thread t{[](int& ri, const Object& obj) {
               ri += 1;
               obj.f(ri);
             },
             std::ref(i), std::ref(object)};
    t.join();
  }
  return 0;
}
/* output
f()
f(1, 3.14)
Object::f(101)
*/
