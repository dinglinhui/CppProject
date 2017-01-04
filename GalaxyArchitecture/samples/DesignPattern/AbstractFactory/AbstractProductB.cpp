/*
 * AbstractProductB.cpp
 *
 *  Created on: Nov 11, 2014
 *      Author: dlh
 */

#include "AbstractProductB.h"

namespace DesignPattern {
namespace AbstractFactoryPattern {

//AbstractProductB
AbstractProductB::AbstractProductB() {
	std::cout << "AbstractProductB..." << std::endl;
}

AbstractProductB::~AbstractProductB() {
	std::cout << "~AbstractProductB..." << std::endl;
}

} /* namespace AbstractFactoryPattern */
} /* namespace DesignPattern */
