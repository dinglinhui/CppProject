/*
 * CommCommand.hpp
 *
 *  Created on: Apr 8, 2015
 *      Author: dinglinhui
 */

#ifndef COMMCOMMAND_HPP_
#define COMMCOMMAND_HPP_

#include <functional>
#include <type_traits>

namespace OSUtils {

template<typename R = void>
struct CommCommand {
private:
	std::function<R()> m_f;

public:
	template<class F, class ... Args, class = typename std::enable_if<
			!std::is_member_function_pointer<F>::value>::type>
	void Wrap(F && f, Args && ... args) {
		m_f = [&] {return f(args...);};
	}

	template<class C, class ... DArgs, class P, class ... Args>
	void Wrap(R (C::*f)(DArgs...) const, P && p, Args && ... args) {
		m_f = [&, f] {return (*p.*f)( args...);};
	}

	// non-const member function
	template<class C, class ... DArgs, class P, class ... Args>
	void Wrap(R (C::*f)(DArgs...), P && p, Args && ... args) {
		m_f = [&, f] {return (*p.*f)( args...);};
	}

	R Excecute() {
		return m_f();
	}
};
} //namespace OSUtils

namespace test {
struct STA {
	int m_a;
	int operator()() {
		return m_a;
	}
	int operator()(int n) {
		return m_a + n;
	}
	int triple0() {
		return m_a * 3;
	}
	int triple(int a) {
		return m_a * 3 + a;
	}
	int triple1() const {
		return m_a * 3;
	}
	const int triple2(int a) const {
		return m_a * 3 + a;
	}

	void triple3() {
		std::cout << "" << std::endl;
	}
};

int add_one(int n) {
	return n + 1;
}

void testWrap() {
	std::cout << "testWrap:" << std::endl;

	OSUtils::CommCommand<int> cmd;
	// free function
	cmd.Wrap(add_one, 0);

	// lambda function
	cmd.Wrap([](int n) {return n + 1;}, 1);

	// functor
//	cmd.Wrap(bloop);
//	cmd.Wrap(bloop, 4);

	STA t = { 10 };
	int x = 3;
	// member function
	cmd.Wrap(&STA::triple0, &t);
	cmd.Wrap(&STA::triple, &t, x);
	cmd.Wrap(&STA::triple, &t, 3);
//	cmd.Wrap(&STA::triple2, &t, 3);

//	auto r = cmd.Excecute();

	OSUtils::CommCommand<> cmd1;
	cmd1.Wrap(&STA::triple3, &t);
	cmd1.Excecute();

	std::cout << std::endl;
}
} //namespace test

#endif /* COMMCOMMAND_HPP_ */
