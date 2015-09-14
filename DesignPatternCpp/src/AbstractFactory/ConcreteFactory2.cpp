/*
 * ConcreteFactory2.cpp
 *
 *  Created on: Nov 11, 2014
 *      Author: dlh
 */

#include "ConcreteFactory2.h"

namespace DesignPattern {
namespace AbstractFactoryPattern {

ConcreteFactory2::ConcreteFactory2() {
	std::cout << "ConcreteFactory2..." << std::endl;
}

ConcreteFactory2::~ConcreteFactory2() {
	std::cout << "~ConcreteFactory2..." << std::endl;
}

AbstractProductA* ConcreteFactory2::CreateProductA() {
	return new ProductA2 { };
}

AbstractProductB* ConcreteFactory2::CreateProductB() {
	return new ProductB2 { };
}

} /* namespace AbstractFactoryPattern */
} /* namespace DesignPattern */
