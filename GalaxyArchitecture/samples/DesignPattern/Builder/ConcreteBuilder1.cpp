/*
 * ConcreteBuilder1.cpp
 *
 *  Created on: Nov 11, 2014
 *      Author: Kevin
 */

#include "ConcreteBuilder1.h"

namespace DesignPattern {
namespace BuilderPattern {

ConcreteBuilder1::ConcreteBuilder1() {
	this->m_pProduct = new Product();
	std::cout << "Create empty product!" << std::endl;
}

void ConcreteBuilder1::BuildPartA() {
	this->m_pProduct->setPartA("A");
	std::cout << "BuildPartA" << std::endl;
}

void ConcreteBuilder1::BuildPartB() {
	this->m_pProduct->setPartB("B");
	std::cout << "BuildPartB" << std::endl;
}

void ConcreteBuilder1::BuildPartC() {
	this->m_pProduct->setPartC("C");
	std::cout << "BuildPartC" << std::endl;
}

Product* ConcreteBuilder1::GetProduct() {
	return this->m_pProduct;
}

ConcreteBuilder1::~ConcreteBuilder1() {
	delete this->m_pProduct;
	this->m_pProduct = nullptr;
}

} /* namespace BuilderPattern */
} /* namespace DesignPattern */
