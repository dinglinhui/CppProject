/*
 * countebase.hpp
 *
 *  Created on: Jun 10, 2015
 *      Author: dlh
 */

#ifndef COUNTEBASE_HPP_
#define COUNTEBASE_HPP_

#include <iostream>

namespace utils {
//一个具有对象计数功能的基类
template<class BeingCounted>
class Counted {
public:
	class TooManyObjects {
	};   //定义一个异常类
	static int objectCount() {
		return numObjects;
	}

protected:
	Counted();
	Counted(const Counted& rhs);
	~Counted() {
		std::cout << "销毁第 " << numObjects << " 个实例" << std::endl;
		--numObjects;
	}

private:
	static int numObjects;           //当前产生实例的个数
	static const size_t maxObjects;  //可以产生实例的最多个数

	void init();
};

template<class BeingCounted>
int Counted<BeingCounted>::numObjects;

template<class Printer>
const size_t Counted<Printer>::maxObjects = 5;   //最多5个

template<class BeingCounted>
Counted<BeingCounted>::Counted() {
	init();
}

template<class BeingCounted>
Counted<BeingCounted>::Counted(const Counted& rhs) {
	init();
}

template<class BeingCounted>
void Counted<BeingCounted>::init() {
	if (numObjects >= maxObjects) {  //产生的实例大于最大值就抛出异常
		throw TooManyObjects();
	}
	std::cout << "构造第 " << numObjects + 1 << " 个实例" << std::endl;
	++numObjects;
}

//继承自具有对象计数功能的基类
class Printer: private Counted<Printer> {
public:
	static Printer* makePrinter();
	static Printer* makePrinter(const Printer& rhs);

	~Printer();
	using Counted<Printer>::objectCount;      //使基类的objectCount在子类可见
	using Counted<Printer>::TooManyObjects;   //使基类的TooManyObjects在子类可见

private:
	Printer();
	Printer(const Printer& rhs);
};

Printer::Printer() {
}

Printer::Printer(const Printer& rhs) {
}

Printer::~Printer() {
}

Printer* Printer::makePrinter() {
	return new Printer;
}

Printer* Printer::makePrinter(const Printer& rhs) {
	return new Printer(rhs);
}
} //namespace utils

namespace test {
using namespace utils;

int testPrinter() {
	Printer *p1 = Printer::makePrinter();
	Printer *p2 = Printer::makePrinter();
	Printer *p3 = Printer::makePrinter(*p1);
	Printer *p4 = Printer::makePrinter(*p2);
	Printer *p5 = Printer::makePrinter();

	delete p4;
	delete p5;
	p4 = Printer::makePrinter();
	p5 = Printer::makePrinter();

	try {
		p1 = Printer::makePrinter(); //Printer产生的实例大于5，则抛出异常
	} catch (Printer::TooManyObjects &) {
		std::cout << "TooManyObjects" << std::endl;
	}

	delete p1;
	delete p2;
	delete p3;
	delete p4;
	delete p5;
	std::cout << "剩余实例为：" << Printer::objectCount() << std::endl;

	return 0;
}
}

#endif /* COUNTEBASE_HPP_ */
