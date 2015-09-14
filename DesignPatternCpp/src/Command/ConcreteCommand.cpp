/*
 * ConcreteCommand.cpp
 *
 *  Created on: Nov 10, 2014
 *      Author: dlh
 */

#include "ConcreteCommand.h"

namespace DesignPattern
{
namespace CommandPattern
{

ConcreteCommand::ConcreteCommand(Receiver* pReceiver)
{
	this->_recv = pReceiver;
}

ConcreteCommand::~ConcreteCommand()
{
}

void ConcreteCommand::Execute()
{
	this->_recv->Action();
}

} /* namespace CommandPattern */
} /* namespace DesignPattern */
