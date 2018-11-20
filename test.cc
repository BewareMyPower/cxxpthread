// test.cc
#include <iostream>
#include "cxxpthread/thread.h"
using namespace std;
using namespace cxxpthread;

struct Object {
  int operator()(int x) {
    cout << "Object::operator()(" << x << ")" << endl;
    x--;
    return x;
  }
  int f(int x) {
    cout << "Object::f(" << x << ")" << endl;
    return x;
  }
};

int main() {
  {
    Thread t([](int x) { return x + 1; }, 1);
    auto p = t.join<int>();
    cout << "  return " << *p << endl;  // 2
  }
  {
    Object obj;
    Thread t(&Object::f, &obj, 2);
    auto p = t.join<int>();
    cout << "  return " << *p << endl;  // 2
  }
  // Can't work in following situations:
  // 1. function with no arguments;
  // 2. function object;
  return 0;
}
