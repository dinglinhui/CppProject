/*
 * ProductB1.cpp
 *
 *  Created on: Nov 11, 2014
 *      Author: dlh
 */

#include "ProductB1.h"

namespace DesignPattern {
namespace AbstractFactoryPattern {

//ProductB1
ProductB1::ProductB1() {
	std::cout << "ProductB1..." << std::endl;
}

ProductB1::~ProductB1() {
	std::cout << "~ProductB1..." << std::endl;
}

void ProductB1::operation() {
}

} /* namespace AbstractFactoryPattern */
} /* namespace DesignPattern */
