/*
 * Colleage.h
 *
 *  Created on: Nov 10, 2014
 *      Author: dlh
 */
#pragma once

#include <iostream>
#include "Mediator.h"

namespace DesignPattern {
namespace MediatorPattern {
class Mediator;
class Colleage {
public:
	virtual ~Colleage();
	virtual void SetMediator(Mediator* pMediator);
	virtual void SendMsg(std::string msg) = 0;
	virtual void GetMsg(std::string msg) = 0;

protected:
	Colleage(Mediator* pMediator);
	Mediator* _mediator = nullptr;
};

} /* namespace MediatorPattern */
} /* namespace DesignPattern */
