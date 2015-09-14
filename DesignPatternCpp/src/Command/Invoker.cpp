/*
 * Invoker.cpp
 *
 *  Created on: Nov 10, 2014
 *      Author: dlh
 */

#include "Invoker.h"

namespace DesignPattern
{
namespace CommandPattern
{

Invoker::Invoker(Command* pCommand)
{
	this->_cmd = pCommand;
}

Invoker::~Invoker()
{
}

void Invoker::Invoke()
{
	this->_cmd->Execute();
}

} /* namespace CommandPattern */
} /* namespace DesignPattern */
