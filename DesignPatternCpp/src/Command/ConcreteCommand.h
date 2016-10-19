/*
 * ConcreteCommand.h
 *
 *  Created on: Nov 10, 2014
 *      Author: dlh
 */

#pragma once

#include "Receiver.h"
#include "Command.h"

namespace DesignPattern {
namespace CommandPattern {

class Receiver;
class ConcreteCommand: public Command {
public:
	ConcreteCommand(Receiver* pReceiver);
	~ConcreteCommand();

	virtual void Execute() override;

private:
	Receiver* _recv = nullptr;
};

} /* namespace CommandPattern */
} /* namespace DesignPattern */
