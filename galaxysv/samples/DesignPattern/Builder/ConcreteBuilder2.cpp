/*
 * ConcreteBuilder2.cpp
 *
 *  Created on: Nov 11, 2014
 *      Author: Kevin
 */

#include "ConcreteBuilder2.h"

namespace DesignPattern {
namespace BuilderPattern {

ConcreteBuilder2::ConcreteBuilder2() {
	this->m_pProduct = new Product();
	std::cout << "Create empty product!" << std::endl;
}

void ConcreteBuilder2::BuildPartA() {
	this->m_pProduct->setPartA("A");
	std::cout << "BuildPartA" << std::endl;
}

void ConcreteBuilder2::BuildPartB() {
	this->m_pProduct->setPartB("B");
	std::cout << "BuildPartB" << std::endl;
}

void ConcreteBuilder2::BuildPartC() {
	this->m_pProduct->setPartC("C");
	std::cout << "BuildPartC" << std::endl;
}

Product* ConcreteBuilder2::GetProduct() {
	return this->m_pProduct;
}

ConcreteBuilder2::~ConcreteBuilder2() {
	delete this->m_pProduct;
	this->m_pProduct = nullptr;
}

} /* namespace BuilderPattern */
} /* namespace DesignPattern */
