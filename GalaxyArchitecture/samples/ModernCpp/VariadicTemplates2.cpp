/*
 * VariadicTemplates.cpp
 *
 *  Created on: Oct 13, 2014
 *      Author: xdinlin
 */
#include <iostream>     // std::cout
#include <tuple>        // std::tuple, std::get, std::tie, std::ignore

/************************************************************************************************/
//declaration
template<long ... nums> struct Multiply;
//recursion
template<long first, long ... last> struct Multiply<first, last...> {
	static const long val = first * Multiply<last...>::val;
};
//border
template<> struct Multiply<> {
	static const long val = 1;
};

/************************************************************************************************/
template<typename ... Elements> struct Tuple;
template<typename Head, typename ... Tail> struct Tuple<Head, Tail...> : private Tuple<
		Tail...> {
	Head head;
};
template<> struct Tuple<> {
};

/************************************************************************************************/
template<typename I, template<typename > class...B> struct Container;
template<typename I, template<typename > class A, template<typename > class ...B> struct Container<
		I, A, B...> {
	A<I> a;
	Container<I, B...> b;
};

template<typename I> struct Container<I> {
};

#if 0

int main()
{
	std::cout << Multiply<2, 3, 4, 5>::val << std::endl;

	//Variadic Templates apply
	std::tuple<int, char> foo(10, 'x');

	auto bar = std::make_tuple("test", 3.1, 14, 'y');
	std::get < 2 >(bar) = 100;// access element

	int myint = 0;
	char mychar = ' ';

	std::tie(myint, mychar) = foo;// unpack elements
	std::tie(std::ignore, std::ignore, myint, mychar) = bar;// unpack (with ignore)

	std::get < 0 >(foo) = std::get < 2 >(bar);
	std::get < 1 >(foo) = std::get < 3 >(bar);

	std::cout << "foo contains: ";
	std::cout << std::get < 0 >(foo) << ' ';
	std::cout << std::get < 1 >(foo) << '\n';

	std::cout << "bar contains: ";
	std::cout << std::get < 0 >(bar) << ' ';
	std::cout << std::get < 1 >(bar) << ' ';
	std::cout << std::get < 2 >(bar) << ' ';
	std::cout << std::get < 3 >(bar) << '\n';

	return 0;
}

#endif
