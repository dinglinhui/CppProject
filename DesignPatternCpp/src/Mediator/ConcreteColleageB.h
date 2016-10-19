/*
 * ConcreteColleageB.h
 *
 *  Created on: Nov 10, 2014
 *      Author: dlh
 */
#pragma once

#include "Mediator.h"
#include "Colleage.h"

namespace DesignPattern {
namespace MediatorPattern {

class Mediator;
class ConcreteColleageB: public Colleage {
public:
	ConcreteColleageB(Mediator* pMediator);
	~ConcreteColleageB();

	virtual void SendMsg(std::string msg) override;
	virtual void GetMsg(std::string msg) override;
};

} /* namespace MediatorPattern */
} /* namespace DesignPattern */

