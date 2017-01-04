#include "State.h"
#include <iostream>

namespace DesignPattern {
namespace StatePattern {

State::State() {
}

State::~State() {
}

ConcreteStateA::ConcreteStateA() {
}

ConcreteStateA::~ConcreteStateA() {
}

void ConcreteStateA::Handle(Context* pContext) {
	std::cout << "ConcreteStateA" << std::endl;
	pContext->ChangeState(new ConcreteStateB());
}

ConcreteStateB::ConcreteStateB() {
}

ConcreteStateB::~ConcreteStateB() {
}

void ConcreteStateB::Handle(Context* pContext) {
	std::cout << "ConcreteStateB" << std::endl;
	pContext->ChangeState(new ConcreteStateC());
}

ConcreteStateC::ConcreteStateC() {
}

ConcreteStateC::~ConcreteStateC() {
}

void ConcreteStateC::Handle(Context* pContext) {
	std::cout << "ConcreteStateC" << std::endl;
	pContext->ChangeState(new ConcreteStateA());
}

Context::Context(State* pState) {
	this->_state = pState;
}

Context::~Context() {
}

void Context::Request() {
	if (nullptr != this->_state) {
		this->_state->Handle(this);
	}
}

void Context::ChangeState(State* pState) {
	this->_state = pState;
}

} /* namespace StatePattern */
} /* namespace DesignPattern */
