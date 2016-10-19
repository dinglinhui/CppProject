#include "Adaptee.h"

namespace DesignPattern {
namespace AdapterPattern {

Adaptee::Adaptee() {
}

Adaptee::~Adaptee() {
}

void Adaptee::SpecificRequest() {
	cout << "Adaptee::SpecificRequest()" << endl;
}

} /* namespace AdapterPattern */
} /* namespace DesignPattern */
