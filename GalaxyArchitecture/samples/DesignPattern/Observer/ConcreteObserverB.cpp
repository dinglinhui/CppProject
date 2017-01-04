#include "ConcreteObserverB.h"

namespace DesignPattern {
namespace ObserverPattern {

ConcreteObserverB::ConcreteObserverB() {
}

ConcreteObserverB::~ConcreteObserverB() {
}

void ConcreteObserverB::Update(Subject* pSubject) {
	this->m_state = pSubject->GetState();
	std::cout << "The ConcreteObserverB is " << m_state << std::endl;
}

} /* namespace ObserverPattern */
} /* namespace DesignPattern */
