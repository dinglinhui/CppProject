/*
 * ProductA1.cpp
 *
 *  Created on: Nov 11, 2014
 *      Author: dlh
 */

#include "ProductA1.h"

namespace DesignPattern {
namespace AbstractFactoryPattern {

//ProductA1
ProductA1::ProductA1() {
	std::cout << "ProductA1..." << std::endl;
}

ProductA1::~ProductA1() {
	std::cout << "~ProductA1..." << std::endl;
}

void ProductA1::operation() {
}

} /* namespace AbstractFactoryPattern */
} /* namespace DesignPattern */
