/*
 * ConcreteAbstractionImplementA.cpp
 *
 *  Created on: Nov 11, 2014
 *      Author: dlh
 */

#include "ConcreteAbstractionImplementA.h"

namespace DesignPattern
{
namespace BridgePattern
{

ConcreteAbstractionImplementA::ConcreteAbstractionImplementA()
{
}

ConcreteAbstractionImplementA::~ConcreteAbstractionImplementA()
{
}

void ConcreteAbstractionImplementA::Operation()
{
	std::cout << "ConcreteAbstractionImplementA Operation" << std::endl;
}

} /* namespace BridgePattern */
} /* namespace DesignPattern */
