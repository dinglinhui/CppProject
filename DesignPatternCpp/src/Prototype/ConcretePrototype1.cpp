/*
 * ConcretePrototype1.cpp
 *
 *  Created on: Nov 12, 2014
 *      Author: dlh
 */

#include "ConcretePrototype1.h"

namespace DesignPattern
{
namespace PrototypePattern
{

ConcretePrototype1::ConcretePrototype1()
{
	std::cout << "ConcretePrototype1" << std::endl;
}

ConcretePrototype1::~ConcretePrototype1()
{
	std::cout << "~ConcretePrototype1" << std::endl;
}

ConcretePrototype1::ConcretePrototype1(const ConcretePrototype1& cp)
{
	std::cout << "ConcretePrototype1 copy" << std::endl;
}

Prototype* ConcretePrototype1::Clone() const
{
	return new ConcretePrototype1(*this);
}

} /* namespace PrototypePattern */
} /* namespace DesignPattern */
