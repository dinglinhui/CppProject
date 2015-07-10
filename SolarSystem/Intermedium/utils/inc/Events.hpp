/*
 * Events.hpp
 *
 *  Created on: Apr 8, 2015
 *      Author: dinglinhui
 */

#ifndef EVENTS_HPP_
#define EVENTS_HPP_

#include <map>
#include <iostream>
#include <functional>
#include <type_traits>
#include "Restriction.hpp"

namespace utils {
template<typename Func>
class Events: NonCopyable {
public:
	Events() :
			m_nextKey(0) {
	}

	~Events() {
	}

	int operator +=(Func&& f) {
		return Connect(std::forward < Func > (f));
	}

	int operator +=(Func& f) {
		return Connect(f);
	}

	template<typename ... Args>
	void operator()(Args&&... args) {
		Notify(std::forward<Args>(args)...);
	}

	Events& operator -=(int key) {
		Disconnect(key);
		return *this;
	}

	void Clear() {
		m_connections.clear();
	}

private:
	int Connect(Func&& f) {
		return Assgin(std::forward < Func > (f));
	}

	int Connect(Func& f) {
		return Assgin(f);
	}

	void Disconnect(int key) {
		m_connections.erase(key);
	}

	template<typename ... Args>
	void Notify(Args&&... args) {
		for (auto& it : m_connections) {
			it.second(std::forward<Args>(args)...);
		}
	}

	template<typename F>
	int Assgin(F&& f) {
		int index = m_nextKey++;
		Push(index, std::forward < Func > (f));
		return index;
	}

	template<typename F>
	void Push(int index, F&& f) {
		m_connections.emplace(index, std::move(f));
	}

	template<typename F>
	void Push(int index, F& f) {
		m_connections.emplace(index, f);
	}

private:
	int m_nextKey;
	std::map<int, Func> m_connections;
};

} //namespace utils

#if 0
namespace test {
	using namespace utils;

	struct STA {
		int a;
		int b;
		void fun(int x, int y) {
			a = x;
			b = y;
			std::cout << "key3:" << a << ", " << b << std::endl;
		}
	};

	void print(int a, int b) {
		std::cout << "key1:" << a << ", " << b << std::endl;
	}

	void testEvents() {
		std::cout << "testEvents:" << std::endl;

		using Delegate = std::function<void(int, int)>;
		using Event = Events<Delegate>;
		Event event;

		//添加委托
		STA t;
		auto key1 = event += &print;
		auto key2 = event += [&t](int a, int b) {
			t.a = a;
			t.b = b;
			std::cout << "key2:" << t.a << ", " << t.b << std::endl;
		};
		auto key3 = event += std::bind(&STA::fun, &t, std::placeholders::_1, std::placeholders::_2);

		//广播
		event(2, 3);

		//移除委托
		event -= key1;
		event -= key2;

		event(4, 5);
		//清空事件
		event -= key3;
		event.Clear();
		event(1, 2);//清空什么都不会输出

		std::cout << std::endl;
	}
} //namespace test
#endif

#endif /* EVENTS_HPP_ */
