//============================================================================
// Name        : VitalCpp.cpp
// Author      : Kevin Ding
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================
#if 0
#include <iostream>
#include <vector>
#include <cstring>
#include <climits>
#include <cstdio>
#include <type_traits>
#include <algorithm>
#include <stdexcept>
#include "Range.hpp"

#define assert_static(e) \
	do { \
		enum { assert_static__ = 1/(e) }; \
	}while(0)

#define LOG(...) { \
	fprintf(stderr, "%s: Line %d:\t", __FILE__, __LINE__);\
	fprintf(stderr, __VA_ARGS__);\
	fprintf(stderr, "\n");\
}

/*#define CONCAT(x) PRAGMA(concat on #x)
 #define PRAGMA(x) _Pragma(#x)
 CONCAT( ..\concat.dir )*/
enum class Color
{
	RED, BLUE
};

enum class Fruit
{
	BANANA, APPLE
};

enum FeatureSupports
{
	C99 = 0x0001,
	ExtInt = 0x0002,
	SAssert = 0x0004,
	NoExcept = 0x0008,
	SMAX = 0x0010,
};

template<typename T, typename U> void bit_copy(T &a, U &b)
{
//	assert_static(sizeof(a) == sizeof(b));
	static_assert(sizeof(a) == sizeof(b), "the parameters of bit_copy must have same width");
	memcpy(&a, &b, sizeof(b));
}

static_assert((SMAX-1) == (C99 | ExtInt | SAssert | NoExcept), "enum values have the wrong set");

/*int (*(*pf())())()
 {
 return nullptr;
 }

 auto pf1() -> auto (*)() -> int (*)()
 {
 return nullptr;
 }

 auto fee() -> void
 {

 }*/

/************************************************************************************************/
constexpr int CFibonacci(int n)
{
	return (n == 1) ? 1 : ((n == 2) ? 1 : CFibonacci(n - 1) + CFibonacci(n - 2));
}

template<long num> struct Fibonacci
{
	static const long val = Fibonacci<num - 1>::val + Fibonacci<num - 2>::val;
};

template<> struct Fibonacci<2>
{
	static const long val = 1;
};

template<> struct Fibonacci<1>
{
	static const long val = 1;
};

template<> struct Fibonacci<0>
{
	static const long val = 0;
};

/************************************************************************************************/

//declaration
template<long ... nums> struct Multiply;
//recursion
template<long first, long ... last> struct Multiply<first, last...>
{
	static const long val = first * Multiply<last...>::val;
};
//border
template<> struct Multiply<>
{
	static const long val = 1;
};

//
template<typename ... Elements> class Tuple;
template<typename Head, typename ... Tail> class Tuple<Head, Tail...> : private Tuple<
		Tail...>
{
	Head head;
};
template<> class Tuple<>
{

};

template<typename ... T> void func(T ... args)
{
}

/************************************************************************************************/
struct A
{
	A()
	{
	}
	A(const A& a)
	{
		cout << "Copy Constructed" << __func__ << endl;
	}
	A(A && a)
	{
		cout << "Move Constructed" << __func__ << endl;
	}
};

struct B
{
	B()
	{
	}
	B(const B& b)
	{
		cout << "Copy Constructed" << __func__ << endl;
	}
	B(B && b)
	{
		cout << "Move Constructed" << __func__ << endl;
	}
};

template<typename ...T> struct MutliTypes;

template<typename T1, typename ...T> struct MutliTypes<T1, T...> : public MutliTypes<
		T...>
{
	T1 t1;
	MutliTypes<T1, T...>(T1 a, T ... b) :
			t1(a), MutliTypes<T...>(b...)
	{
		cout << "MutliTypes<T1, T...>(T1 a, T ... b)" << endl;
	}
};

template<> struct MutliTypes<>
{
	MutliTypes<>()
	{
		cout << "MutliTypes<>()" << endl;
	}
};

template<template<typename ...> class VariadicType, typename...Args>
VariadicType<Args...> Build(Args&& ...args)
{
	return VariadicType<Args...> (std::forward<Args>(args)...);
}

/************************************************************************************************/
template<typename I, template<typename > class...B> struct Container;
template<typename I, template<typename > class A, template<typename > class ...B> struct Container<
		I, A, B...>
{
	A<I> a;
	Container<I, B...> b;
};

template<typename I> struct Container<I>
{
};
/************************************************************************************************/
template<typename A, typename B> struct S
{
};

template<template<typename ...> class T, typename ...TArgs,template<typename ...> class U, typename ...UArgs> struct S<T<TArgs...>, U<UArgs...>>
{

};
/************************************************************/
void Printf(const char * s)
{
	while (*s)
	{
		if (*s == '%' && *++s != '%')
			throw runtime_error("invalid format string: missing arguments");
		cout << *s++;
	}
}

template<typename T, typename ... Args> void Printf(const char * s, T value,
		Args ... args)
{
	while (*s)
	{
		if (*s == '%' && *++s != '%')
		{
			cout << value;
			return Printf(++s, args...);
		}
		cout << *s++;
	}
	throw runtime_error("extra arguments provided to Printf");
}

int main()
{

	/*int a = 0x2468;
	 double b;
	 bit_copy(a, b);*/

	/*cout << sizeof(((CTest *) 0)->m_nTest) << endl;
	 cout << sizeof(CTest::m_nTest) << endl;
	 cout << sizeof(CTest::ptr) << endl;*/

	cout << "Standard CLib:" << __STDC_HOSTED__ << endl;
	cout << "Standard C:" << __STDC__ << endl;
	//	cout << "ISO/IEC " << __STDC_ISO_10646 <<endl;
	//	cout << "C Standard Version:" << __STDC_VERSION__ <<endl;
	cout << __cplusplus << endl;
	cout << __func__ << endl;

	long long ll_min = LLONG_MIN;
	long long ll_max = LLONG_MAX;
	unsigned long long ull_max = ULLONG_MAX;
	cout << ll_min << ", " << ll_max << ", " << ull_max << endl;

	int x = 3;
	LOG("x = %d", x);

	Color a = Color::RED; // note: RED is not accessible any more, we have to use Color::RED
	Fruit b = Fruit::BANANA; // note: BANANA is not accessible any more, we have to use Fruit::BANANA
	if (a == b) // compile error here, as the compiler doesn't know how to compare different types Color and Fruit
		cout << "a and b are equal" << endl;
	else
		cout << "a and b are not equal" << endl;

//	for (auto x : lots::Range(1, 5, 3))
//	{
//		std::cout << x << std::endl;
//	}
//	vector<int> v
//	{ 1, 2, 3, 4, 5 };
//	for (auto i = std::begin(v); i != std::end(v); i++)
//	{
//		std::cout << *i << std::endl;
//	}
//	for (auto e : v)
//	{
//		std::cout << e << std::endl;
//	}
//	std::for_each(std::begin(v), std::end(v), [](int x) {
//		std::cout << x;
//	});
	int foo[12]
	{ 0 };
	std::vector<int> bar(
	{ 10, 20, 30, 40, 50 });

	decltype(foo) arr;
	std::cout << std::end(arr) - std::begin(arr) << std::endl;
	// iterate bar: print contents:
//	std::cout << "bar contains:";
//	for (auto it = std::begin(bar); it != std::end(bar); ++it)
//		std::cout << *it << " ";
	std::for_each(std::begin(bar), std::end(bar), [](int it)
	{
		std::cout << it << " ";
	});

// iterate foo: inserting into bar
	for (auto it = std::begin(foo); it != std::end(foo); ++it)
		std::cout << *it << " ";

	std::cout << std::endl << is_same<decltype(pf), decltype(pf1)>::value
			<< std::endl;

	std::cout << Multiply<2, 3, 4, 5>::val << std::endl;
	Printf("Hello, %s, %s\n", string("Nice to meet you!"), string("Kevin"));
	return 0;
}
#endif
