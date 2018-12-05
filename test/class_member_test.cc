// class_member_test.cc
#include "thread.h"
using namespace cxxpthread;

#include <iostream>  // std::cout
using namespace std;

struct Object {
  void f() { cout << "Object::f()" << endl; }
  void g(int i) { cout << "Object::g(" << i << ")" << endl; }
};

int main() {
  Object object;
  {
    Thread t{&Object::f, &object};
    t.join();
  }
  {
    Thread t{&Object::g, &object, 100};
    t.join();
  }
  return 0;
}
