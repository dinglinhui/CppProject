/*
 * ConcreteDecoratorA.cpp
 *
 *  Created on: Nov 12, 2014
 *      Author: dlh
 */

#include "ConcreteDecoratorA.h"

namespace DesignPattern
{
namespace DecoratorPattern
{

ConcreteDecoratorA::ConcreteDecoratorA(Component* com) :
		Decorator(com)
{
}

ConcreteDecoratorA::~ConcreteDecoratorA()
{
	std::cout << "~ConcreteDecoratorA" << std::endl;
}

void ConcreteDecoratorA::Operation()
{
	this->_com->Operation();
	this->AddBehavorA();
}

void ConcreteDecoratorA::AddBehavorA()
{
	std::cout << "ConcreteDecoratorA::AddBehavorA" << std::endl;
}


} /* namespace DecoratorPattern */
} /* namespace DesignPattern */
