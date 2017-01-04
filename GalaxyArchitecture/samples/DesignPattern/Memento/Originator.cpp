/*
 * Originator.cpp
 *
 *  Created on: Nov 12, 2014
 *      Author: dlh
 */

#include "Originator.h"

namespace DesignPattern {
namespace MementoPattern {

Originator::Originator() {
}

Originator::Originator(const std::string& state) {
	this->_state = state;
}

Originator::~Originator() {
}

std::string Originator::GetState() {
	return this->_state;
}

void Originator::show() {
	std::cout << this->_state << std::endl;
}

void Originator::SetState(const std::string& state) {
	this->_state = state;
}

Memento* Originator::CreateMemento() {
	return new Memento(this->_state);
}

void Originator::RestoreToMemento(Memento* pMemento) {
	this->_state = pMemento->GetState();
}

} /* namespace MementoPattern */
} /* namespace DesignPattern */
