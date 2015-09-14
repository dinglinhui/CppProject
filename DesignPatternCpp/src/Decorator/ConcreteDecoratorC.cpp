/*
 * ConcreteDecoratorC.cpp
 *
 *  Created on: Nov 12, 2014
 *      Author: dlh
 */

#include "ConcreteDecoratorC.h"

namespace DesignPattern
{
namespace DecoratorPattern
{

ConcreteDecoratorC::ConcreteDecoratorC(Component* com) :
		Decorator(com)
{
}

ConcreteDecoratorC::~ConcreteDecoratorC()
{
	std::cout << "~ConcreteDecoratorC" << std::endl;
}

void ConcreteDecoratorC::Operation()
{
	this->_com->Operation();
	this->AddBehavorC();
}

void ConcreteDecoratorC::AddBehavorC()
{
	std::cout << "ConcreteDecoratorC::AddBehavorC" << std::endl;
}

} /* namespace DecoratorPattern */
} /* namespace DesignPattern */
