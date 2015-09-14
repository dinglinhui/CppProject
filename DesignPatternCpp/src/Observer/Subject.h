#pragma once
#include <list>
#include <string>
#include <iostream>
#include "Observer.h"

namespace DesignPattern
{
namespace ObserverPattern
{

class Observer;
class Subject
{
public:
	virtual ~Subject();
	virtual void Notify();
	virtual void Attach(Observer*);
	virtual void Detach(Observer*);
	virtual std::string GetState();
	virtual void SetState(std::string state);
protected:
	Subject();
private:
	std::string m_state;
	std::list<Observer*> m_lst;
};

} /* namespace ObserverPattern */
} /* namespace DesignPattern */
