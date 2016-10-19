/*
 * ConcreteComponent.cpp
 *
 *  Created on: Nov 12, 2014
 *      Author: dlh
 */

#include "ConcreteComponent.h"

namespace DesignPattern {
namespace DecoratorPattern {

ConcreteComponent::ConcreteComponent() {
}

ConcreteComponent::~ConcreteComponent() {
	std::cout << "~ConcreteComponent" << std::endl;
}

void ConcreteComponent::Operation() {
	std::cout << "ConcreteComponent::Operation" << std::endl;
}

} /* namespace DecoratorPattern */
} /* namespace DesignPattern */
