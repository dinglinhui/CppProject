/*
 * Originator.h
 *
 *  Created on: Nov 12, 2014
 *      Author: dlh
 */

#pragma once

#include "Memento.h"
namespace DesignPattern {
namespace MementoPattern {

class Memento;
class Originator {
public:
	Originator();
	Originator(const std::string& state);
	~Originator();

	void RestoreToMemento(Memento* pMemento);
	Memento* CreateMemento();
	void SetState(const std::string& state);
	std::string GetState();
	void show();
private:
	std::string _state;
};

} /* namespace MementoPattern */
} /* namespace DesignPattern */
