#include "Facade.h"

namespace DesignPattern {
namespace FacadePattern {

Facade::Facade() {
	this->_subsys1 = new Subsystem1();
	this->_subsys2 = new Subsystem2();
}

Facade::~Facade() {
	delete this->_subsys1;
	delete this->_subsys2;

	this->_subsys1 = nullptr;
	this->_subsys2 = nullptr;
}

void Facade::OperationWrapper() {
	this->_subsys1->Operation();
	this->_subsys2->Operation();
}

} /* namespace FacadePattern */
} /* namespace DesignPattern */
