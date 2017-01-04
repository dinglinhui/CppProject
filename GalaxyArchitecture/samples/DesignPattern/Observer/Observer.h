#pragma once

namespace DesignPattern {
namespace ObserverPattern {

class Subject;
class Observer {
public:
	virtual ~Observer();
	virtual void Update(Subject*) = 0;

protected:
	Observer();
};

} /* namespace ObserverPattern */
} /* namespace DesignPattern */
