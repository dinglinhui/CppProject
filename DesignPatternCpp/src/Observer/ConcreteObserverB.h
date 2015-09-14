#pragma once

#include "Subject.h"
#include "Observer.h"
namespace DesignPattern
{
namespace ObserverPattern
{
class Subject;
class ConcreteObserverB: public Observer
{
public:
	ConcreteObserverB();
	virtual ~ConcreteObserverB();
	virtual void Update(Subject*);
private:
	std::string m_state;
};

} /* namespace ObserverPattern */
} /* namespace DesignPattern */
