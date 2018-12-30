# cxxpthread
A C++11 wrapper of pthread library, inspired by c++11 `<thread>`.

It's a header-only library, and only requires compiler which supports C++11. (eg. gcc 4.8)

See examples in [test](./test) to learn how it works.
Here we'll give some simple examples, use `cxxpthread::Thread` need 2 or 3 steps:
- 1. create
- 2. join
- 3. getResult() or getExitCode()

# 1. Create thread
## 1.1 from C-style functions
```
void f(int, int&);
int i, j;

Thread t(f, i, std::ref(j));
```

## 1.2 from function object
```
struct Foo {
  void operator(int, int&);
};
Foo foo;
int i, j;

Thread t(foo, i, std::ref(j));
// NOTE: pass foo to Thread constructor means pass a copy of foo
```

## 1.3 from class member function
```
struct Foo {
  void f(int, int&);
};
Foo foo;
int i, j;

Thread t(&Foo::f, &foo, i, std::ref(j));
```

# 2. Join thread
After `Thread` object `t` was created, you must call `t.join()` later, which internally calls `pthread_join()` and save result of `pthread_exit`.

# 3. Get result of thread function
This step is not necessary, but if you want to get return value or exit code of thread function, you need it.
## 3.1 Return value of thread function
```
int f() { return 100; }

Thread t(f);
t.join();
auto p = t.getResult<int>(t);  // p is std::unique_ptr<int> object
```
**NOTE**
1. If you don't call `Thread::getResult<T>()` for a function with return value, it will cause memory leak!
2. `Thread::getResult<T>()` can only be called once!
3. If `T` is not proper, it may cause memory leak, like `int* p = new int(1); delete (double*)p; `;
4. `T` must be moveable.

## 3.2 Call `Thread::exit()` in thread function and get thread exit code by `Thread::getExitCode`
```
void f() { Thread::exit(1); }

Thread t(f);
t.join();
long exit_code = t.getExitCode();  // 1
```
