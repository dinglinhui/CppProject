/*
 * ConcreteColleageA.cpp
 *
 *  Created on: Nov 10, 2014
 *      Author: dlh
 */

#include "ConcreteColleageA.h"

namespace DesignPattern
{
namespace MediatorPattern
{

ConcreteColleageA::ConcreteColleageA(Mediator* pMediator) :
		Colleage(pMediator)
{
}

ConcreteColleageA::~ConcreteColleageA()
{
}

void ConcreteColleageA::SendMsg(std::string msg)
{
	this->_mediator->SendMsg(msg, this);
}

void ConcreteColleageA::GetMsg(std::string msg)
{
	std::cout << "ConcreteColleageA Receive:" << msg << std::endl;
}

} /* namespace MediatorPattern */
} /* namespace DesignPattern */
