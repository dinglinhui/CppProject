/*
 * ProductB2.cpp
 *
 *  Created on: Nov 11, 2014
 *      Author: dlh
 */

#include "ProductB2.h"

namespace DesignPattern {
namespace AbstractFactoryPattern {

//ProductB2
ProductB2::ProductB2() {
	std::cout << "ProductB2..." << std::endl;
}

ProductB2::~ProductB2() {
	std::cout << "~ProductB2..." << std::endl;
}

void ProductB2::operation() {
}

} /* namespace AbstractFactoryPattern */
} /* namespace DesignPattern */
