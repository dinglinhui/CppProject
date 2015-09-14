#pragma once

#include "Observer.h"
#include "Subject.h"

namespace DesignPattern
{
namespace ObserverPattern
{
class Subject;
class ConcreteObserverA: public Observer
{
public:
	ConcreteObserverA();
	virtual ~ConcreteObserverA();
	virtual void Update(Subject*);
private:
	std::string m_state;
};

} /* namespace ObserverPattern */
} /* namespace DesignPattern */
