#pragma once
#include <iostream>
#include <algorithm>
#include <functional>

int c_function(int a, int b) {
	return a + b;
}

class Functor {
public:
	int operator()(int a, int b) {
		return a + b;
	}
};

void test_functor() {
	int (*f)(int, int) = c_function;
	std::cout << f(3, 4) << std::endl;

	Functor ff = Functor();
	std::cout << ff(3, 4) << std::endl;

	std::function<int(int)> fib = [&fib](int n)	{
		return n < 2 ? 1 : fib(n-1) + fib(n-2);
	};
	std::cout << fib(12) << std::endl;

	// an array of functions:
	std::function<int(int, int)> fn[] = {
		std::plus<int>(),
		std::minus<int>(),
		std::divides<int>(),
		std::multiplies<int>()
	};
	for (auto& x : fn) {
		std::cout << x(10, 5) << '\n';
	}

}

