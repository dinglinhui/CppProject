#pragma once
#include <iostream>
#include <algorithm>
#include <functional>
#include <vector>

namespace basic {

template<typename Iterator>
void get_odds(Iterator begin, Iterator end) {
	std::for_each(begin, end, [](int n) {
		std::cout << n << std::endl;
	});

	auto pos = begin;
	while (pos != end) {
		pos = std::find_if(pos, end, [](int n) {
			return n % 2 == 1;
		});
		std::cout << *pos++ << " ";
	}

	std::cout << std::endl;
}

template<typename C>
void get_odds(C c) {
	get_odds(std::begin(c), std::end(c));
}

template<typename T, size_t N>
void get_odds(T (&arr)[N]) {
	get_odds(std::begin(arr), std::end(arr));
}


template<typename T1, typename T2>
auto add(T1 t1, T2 t2) -> decltype(t1 + t2) {
	static_assert(std::is_integral<T1>::value, "Type T1 must be integral");
	static_assert(std::is_integral<T2>::value, "Type T2 must be integral");
	return t1 + t2;
}

template<typename T>
void func(const T& t) {
	std::cout << t << std::endl;
}

template<typename T, typename ...Args>
void func(const T& t, Args ...args) {
	std::cout << t << std::endl;
	func(args...);
}

template<typename T>
T sum(T v) {
	return v;
}

template<typename T, typename ...Args>
T sum(T v, Args ...args) {
	return v + sum(args...);
}

void printf(const char *s) {
	while (*s) {
		if (*s == '%' && *(++s) != '%')
			throw std::runtime_error(
					"invalid format string: missing arguments");
		std::cout << *s++;
	}
}

template<typename T, typename ... Args>
void printf(const char* s, T value, Args ... args) {
	while (*s) {
		if (*s == '%' && *(++s) != '%') {
			std::cout << value;
			printf(*s ? ++s : s, args...);
			return;
		}
		std::cout << *s++;
	}
	throw std::logic_error("extra arguments provided to printf");
}

template<typename ... Arguments>
class VariadicTemplate {
public:
	void SampleFunction(Arguments ... params) {
	}
};

template<>
class VariadicTemplate<double, int, long> {
public:
	void SampleFunction(double param1, int param2, long param3) {
	}
};

void test_template() {
	int arr[] = { 1, 2, 3 };
	get_odds(arr);

	std::vector<int> v = { 1, 2, 3 };
	get_odds(v);

	func(1, 2, 3, 4, 5);
	std::cout << sum(1, 2, 3, 4, 5) << std::endl;

}
} // namespace basic

