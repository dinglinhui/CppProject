/*
 * ProductA2.cpp
 *
 *  Created on: Nov 11, 2014
 *      Author: dlh
 */

#include "ProductA2.h"

namespace DesignPattern {
namespace AbstractFactoryPattern {

//ProductA2
ProductA2::ProductA2() {
	std::cout << "ProductA2..." << std::endl;
}

ProductA2::~ProductA2() {
	std::cout << "~ProductA2..." << std::endl;
}

void ProductA2::operation() {
}

} /* namespace AbstractFactoryPattern */
} /* namespace DesignPattern */
