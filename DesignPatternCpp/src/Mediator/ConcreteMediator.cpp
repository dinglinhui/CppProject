/*
 * ConcreteMediator.cpp
 *
 *  Created on: Nov 10, 2014
 *      Author: dlh
 */

#include "ConcreteMediator.h"

namespace DesignPattern {
namespace MediatorPattern {

ConcreteMediator::ConcreteMediator() {
}

ConcreteMediator::~ConcreteMediator() {
}

void ConcreteMediator::SetColleageA(Colleage* p) {
	this->m_ColleageA = p;
}

void ConcreteMediator::SetColleageB(Colleage* p) {
	this->m_ColleageB = p;
}

void ConcreteMediator::SendMsg(std::string msg, Colleage* p) {
	if (p == this->m_ColleageA) {
		this->m_ColleageB->GetMsg(msg);
	} else if (p == this->m_ColleageB) {
		this->m_ColleageA->GetMsg(msg);
	}
}

} /* namespace MediatorPattern */
} /* namespace DesignPattern */
