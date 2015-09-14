/*
 * ConcreteDecoratorB.cpp
 *
 *  Created on: Nov 12, 2014
 *      Author: dlh
 */

#include "ConcreteDecoratorB.h"

namespace DesignPattern
{
namespace DecoratorPattern
{

ConcreteDecoratorB::ConcreteDecoratorB(Component* com) :
		Decorator(com)
{
}

ConcreteDecoratorB::~ConcreteDecoratorB()
{
	std::cout << "~ConcreteDecoratorB" << std::endl;
}

void ConcreteDecoratorB::Operation()
{
	this->_com->Operation();
	//����ְ��B
	this->AddBehavorB();
}

void ConcreteDecoratorB::AddBehavorB()
{
	std::cout << "ConcreteDecoratorB::AddBehavorB" << std::endl;
}

} /* namespace DecoratorPattern */
} /* namespace DesignPattern */
