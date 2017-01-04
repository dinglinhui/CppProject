#include "Subject.h"
#include <algorithm>

namespace DesignPattern {
namespace ObserverPattern {

Subject::Subject() {
}

Subject::~Subject() {
}

void Subject::Attach(Observer* pObserver) {
	this->m_lst.push_back(pObserver);
	std::cout << "Attach an Observer\n";
}

void Subject::Detach(Observer* pObserver) {
	std::list<Observer*>::iterator iter;
	iter = std::find(m_lst.begin(), m_lst.end(), pObserver);
	if (iter != m_lst.end()) {
		m_lst.erase(iter);
	}
	std::cout << "Detach an Observer\n";
}

void Subject::Notify() {
	std::list<Observer*>::iterator iter = this->m_lst.begin();
	for (; iter != m_lst.end(); iter++) {
		(*iter)->Update(this);
	}
}

std::string Subject::GetState() {
	return this->m_state;
}

void Subject::SetState(std::string state) {
	this->m_state = state;
}

} /* namespace ObserverPattern */
} /* namespace DesignPattern */
