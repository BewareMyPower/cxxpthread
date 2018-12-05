CXX = g++
TARGETS = c_function_test function_object_test class_member_test thread_exit_test
COMPILEFLAGS = -std=c++11
INCFLAGS = -I../cxxpthread
LIBFLAGS = -pthread

all: $(TARGETS)

c_function_test: c_function_test.cc
	$(CXX) $^ -o $@ $(COMPILEFLAGS) $(INCFLAGS) $(LIBFLAGS)

function_object_test: function_object_test.cc
	$(CXX) $^ -o $@ $(COMPILEFLAGS) $(INCFLAGS) $(LIBFLAGS)

class_member_test: class_member_test.cc
	$(CXX) $^ -o $@ $(COMPILEFLAGS) $(INCFLAGS) $(LIBFLAGS)

thread_exit_test: thread_exit_test.cc
	$(CXX) $^ -o $@ $(COMPILEFLAGS) $(INCFLAGS) $(LIBFLAGS)

clean:
	rm -f $(TARGETS) *.o a.out 
