#ifndef _RVALUE_HPP_
#define _RVALUE_HPP_
#include <iostream>

// function with lvalue and rvalue reference overloads:
void overloaded(const int& x) {
	std::cout << "[lvalue]";
}

void overloaded(int&& x) {
	std::cout << "[rvalue]";
}

// function template taking rvalue reference to deduced type:
template<class T>
void fn(T&& x) {
	overloaded(x);                   // always an lvalue
	overloaded(std::forward<T>(x));  // rvalue if argument is rvalue
}

#endif /* _RVALUE_HPP_ */
