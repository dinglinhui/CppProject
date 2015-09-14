/*
 * Colleage.cpp
 *
 *  Created on: Nov 10, 2014
 *      Author: dlh
 */

#include "Colleage.h"

namespace DesignPattern
{
namespace MediatorPattern
{

Colleage::Colleage(Mediator* pMediator)
{
	this->_mediator = pMediator;
}

Colleage::~Colleage()
{
}

void Colleage::SetMediator(Mediator* pMediator)
{
	this->_mediator = pMediator;
}

} /* namespace MediatorPattern */
} /* namespace DesignPattern */
