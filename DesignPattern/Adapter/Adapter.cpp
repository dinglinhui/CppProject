#include "Adapter.h"
#include <iostream>

namespace DesignPattern {
namespace AdapterPattern {

Adapter::Adapter() {
}

Adapter::~Adapter() {
}

void Adapter::Request() {
	cout << "Adapter::Request()" << endl;
	this->SpecificRequest();
	cout << "----------------------------" << endl;
}

} /* namespace AdapterPattern */
} /* namespace DesignPattern */
