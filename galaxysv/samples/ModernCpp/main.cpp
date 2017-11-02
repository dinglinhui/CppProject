#include <iostream>
#include <cmath>
#include <algorithm>
#include <functional>
#include <type_traits>
#include <cassert>
#include <utility>
#include <numeric>
#include <memory>
#include <string>
#include <sstream>
#include <vector>
#include <future>
#include <unordered_map>

#include "application/employee.hpp"
#include "application/smart_pointer.hpp"
#include "application/student.hpp"
#include "application/utils.hpp"
#include "application/threadpool.hpp"
#include "multithreading/CppThread.hpp"
#include "networking/server.hpp"

// function with lvalue and rvalue reference overloads:
void overloaded(const int& x) { std::cout << "[lvalue]"; }
void overloaded(int&& x) { std::cout << "[rvalue]"; }

// function template taking rvalue reference to deduced type:
template <class T> 
void fn(T&& x) {
    overloaded(x);                   // always an lvalue
    overloaded(std::forward<T>(x));  // rvalue if argument is rvalue
}

int main(int argc, char *argv[]) {
    application::test_threadpool();
#if 0
    application::test_employeesystem();
    application::test_smartpointer();

    multithreading::test_thread();
    multithreading::test_future();
    multithreading::test_promise();
    multithreading::manager();
#endif

    return 0;
}
