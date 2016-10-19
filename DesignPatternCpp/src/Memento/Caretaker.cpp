/*
 * Caretaker.cpp
 *
 *  Created on: Nov 12, 2014
 *      Author: dlh
 */

#include "Caretaker.h"

namespace DesignPattern {
namespace MementoPattern {

Caretaker::Caretaker() {
}

Caretaker::~Caretaker() {
}

Memento* Caretaker::GetMemento() {
	return this->_memento;
}

void Caretaker::SetMemento(Memento* pMemento) {
	this->_memento = pMemento;
}

} /* namespace MementoPattern */
} /* namespace DesignPattern */
