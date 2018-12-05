// function_object_test.cc
#include "thread.h"
using namespace cxxpthread;

#include <functional>  // std::ref
#include <iostream>    // std::cout
using namespace std;

struct Object {
  void operator()() const { cout << "Object()" << endl; }
};

struct ObjectIntDouble {
  void operator()(int i, double d) {  // non const
    cout << "f(" << i << ", " << d << ")" << endl;
  }
};

int main() {
  {
    Object obj;
    Thread t{obj};
    t.join();
  }
  {
    ObjectIntDouble obj;
    Thread t{obj, 1, 3.14};
    t.join();
  }
  // pass ref wrapper
  {
    Object obj;
    auto const& crobj = obj;
    Thread t{std::ref(crobj)};
    t.join();
  }
  // compile error because ObjectIntDouble::operator() is non-const!
//  {
//    ObjectIntDouble obj;
//    auto const& crobj = obj;
//    Thread t{std::ref(crobj), 1, 3.14};
//    t.join();
//  }
  return 0;
}
/*
Object()
f(1, 3.14)
Object()
*/
