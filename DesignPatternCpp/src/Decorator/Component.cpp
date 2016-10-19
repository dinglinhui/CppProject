/*
 * Component.cpp
 *
 *  Created on: Nov 12, 2014
 *      Author: dlh
 */

#include "Component.h"

namespace DesignPattern {
namespace DecoratorPattern {

Component::Component() {
}

Component::~Component() {
	std::cout << "~Component" << std::endl;
}

} /* namespace DecoratorPattern */
} /* namespace DesignPattern */
