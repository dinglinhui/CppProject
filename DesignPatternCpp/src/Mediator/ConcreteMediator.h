/*
 * ConcreteMediator.h
 *
 *  Created on: Nov 10, 2014
 *      Author: dlh
 */
#pragma once

#include "Mediator.h"
#include "Colleage.h"

namespace DesignPattern {
namespace MediatorPattern {

class Colleage;
class ConcreteMediator: public Mediator {
public:
	ConcreteMediator();
	~ConcreteMediator();

	void SetColleageA(Colleage* p);
	void SetColleageB(Colleage* p);

	virtual void SendMsg(std::string msg, Colleage* p) override;

private:
	Colleage* m_ColleageA = nullptr;
	Colleage* m_ColleageB = nullptr;
};

} /* namespace MediatorPattern */
} /* namespace DesignPattern */

