/*
 * DecoratorOnlyOne.cpp
 *
 *  Created on: Nov 12, 2014
 *      Author: dlh
 */

#include "DecoratorOnlyOne.h"

namespace DesignPattern
{
namespace DecoratorPattern
{

DecoratorOnlyOne::DecoratorOnlyOne(Component* com) :
		_com(com)
{
}

DecoratorOnlyOne::~DecoratorOnlyOne()
{
	std::cout << "~DecoratorOnlyOne" << std::endl;
	delete this->_com;
	this->_com = nullptr;
}

void DecoratorOnlyOne::Operation()
{
	this->_com->Operation();
	this->AddBehavor();
}

void DecoratorOnlyOne::AddBehavor()
{
	std::cout << "DecoratorOnlyOne::AddBehavor" << std::endl;
}

} /* namespace DecoratorPattern */
} /* namespace DesignPattern */
