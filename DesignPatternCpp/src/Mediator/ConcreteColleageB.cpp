/*
 * ConcreteColleageB.cpp
 *
 *  Created on: Nov 10, 2014
 *      Author: dlh
 */

#include "ConcreteColleageB.h"

namespace DesignPattern
{
namespace MediatorPattern
{

ConcreteColleageB::ConcreteColleageB(Mediator* pMediator) :
		Colleage(pMediator)
{
}

ConcreteColleageB::~ConcreteColleageB()
{
}

void ConcreteColleageB::SendMsg(std::string msg)
{
	this->_mediator->SendMsg(msg, this);
}

void ConcreteColleageB::GetMsg(std::string msg)
{
	std::cout << "ConcreteColleageB Receive:" << msg << std::endl;
}

} /* namespace MediatorPattern */
} /* namespace DesignPattern */
