/*
 * Invoker.h
 *
 *  Created on: Nov 10, 2014
 *      Author: dlh
 */

#pragma once

#include "Command.h"
namespace DesignPattern
{
namespace CommandPattern
{
class Command;
class Invoker
{
public:
	Invoker(Command* pCommand);
	~Invoker();
	void Invoke();

private:
	Command* _cmd = nullptr;
};

} /* namespace CommandPattern */
} /* namespace DesignPattern */
