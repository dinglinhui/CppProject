/*
 * RightReference.cpp
 *
 *  Created on: Apr 8, 2015
 *      Author: dlh
 */
#include <iostream>
#include <string>
#include <list>

void processValue(int& a) {
	std::cout << "lvalue" << std::endl;
}

void processValue(int&& a) {
	std::cout << "rvalue" << std::endl;
}
template<typename T>
void forwardValue(T&& val) {
//	processValue(val);
	processValue(std::forward < T > (val)); //照参数本来的类型进行转发。
}

template<typename T, typename ... Args>
T* Instance(Args&&... args) {
	return new T(std::forward<Args>(args)...);
}

void Testdelcl() {
	int i = 0;
	forwardValue(i); //传入左值
	forwardValue(0); //传入右值
}
