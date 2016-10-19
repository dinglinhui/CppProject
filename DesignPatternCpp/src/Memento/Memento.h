#pragma once

#include <string>
#include <iostream>
#include "Originator.h"

namespace DesignPattern {
namespace MementoPattern {

class Memento {
private:
	friend class Originator;
	Memento(const std::string& state);
	~Memento();

	void SetState(const std::string& state);
	std::string GetState();
	std::string _state;
};

} /* namespace MementoPattern */
} /* namespace DesignPattern */
