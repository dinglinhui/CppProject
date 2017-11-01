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

#include "application/EmployeeSystem.hpp"
#include "application/Student.h"
#include "application/SmartPointer.hpp"
#include "application/utils.hpp"
#include "multithreading/ThreadPool.hpp"
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
    application::test_smartpointer();

#if 0
    application::test_employeesystem();

    multithreading::test_threadpool();
    multithreading::test_thread();
    multithreading::test_future();
    multithreading::test_promise();
    multithreading::manager();
#endif

    return 0;
}
