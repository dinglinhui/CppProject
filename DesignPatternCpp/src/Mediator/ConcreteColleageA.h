/*
 * ConcreteColleageA.h
 *
 *  Created on: Nov 10, 2014
 *      Author: dlh
 */
#pragma once

#include "Mediator.h"
#include "Colleage.h"
namespace DesignPattern
{
namespace MediatorPattern
{
class Mediator;
class ConcreteColleageA: public Colleage
{
public:
	ConcreteColleageA(Mediator* pMediator);
	~ConcreteColleageA();
	virtual void SendMsg(std::string msg) override;
	virtual void GetMsg(std::string msg) override;
};

} /* namespace MediatorPattern */
} /* namespace DesignPattern */
