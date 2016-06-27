/*
 * stack.cpp
 *
 *  Created on: Jun 3, 2015
 *      Author: dlh
 */

#include <iostream>
#include <vector>
#include <cstdlib>
#include <string>
#include <stdexcept>

namespace utils {

template<class T>
class Stack {
private:
	std::vector<T> elems;     // elements

public:
	void push(T const&);  // push element
	void pop();               // pop element
	T top() const;            // return top element
	bool empty() const {       // return true if empty.
		return elems.empty();
	}
};

template<class T>
void Stack<T>::push(T const& elem) {
	// append copy of passed element
	elems.push_back(elem);
}

template<class T>
void Stack<T>::pop() {
	if (elems.empty()) {
		throw std::out_of_range("Stack<>::pop(): empty stack");
	}
	// remove last element
	elems.pop_back();
}

template<class T>
T Stack<T>::top() const {
	if (elems.empty()) {
		throw std::out_of_range("Stack<>::top(): empty stack");
	}
	// return copy of last element
	return elems.back();
}
}

namespace test {
using namespace utils;

void testStack() {
	std::cout << "testStack:" << std::endl;
	Stack<int> intStack;  // stack of ints
	Stack<std::string> stringStack;    // stack of strings

	// manipulate int stack
	intStack.push(12);
	std::cout << intStack.top() << std::endl;

	// manipulate string stack
	stringStack.push("hello");
	std::cout << stringStack.top() << std::endl;
	stringStack.pop();
//		stringStack.pop();
	std::cout << std::endl;
}
}
