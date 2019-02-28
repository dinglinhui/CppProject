#include "ConcreteObserverA.h"

namespace DesignPattern {
namespace ObserverPattern {

ConcreteObserverA::ConcreteObserverA() {
}

ConcreteObserverA::~ConcreteObserverA() {
}

void ConcreteObserverA::Update(Subject* pSubject) {
	this->m_state = pSubject->GetState();
	std::cout << "The ConcreteObserverA is " << m_state << std::endl;
}

} /* namespace ObserverPattern */
} /* namespace DesignPattern */
