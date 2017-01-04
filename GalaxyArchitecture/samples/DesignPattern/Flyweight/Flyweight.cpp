#include "Flyweight.h"

namespace DesignPattern {
namespace FlyweightPattern {

Flyweight::Flyweight(std::string intrinsicState) {
	this->_intrinsicState = intrinsicState;
}

Flyweight::~Flyweight() {
}

std::string Flyweight::GetIntrinsicState() {
	return this->_intrinsicState;
}

} /* namespace FlyweightPattern */
} /* namespace DesignPattern */
