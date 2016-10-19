#pragma once

#include "Target.h"
#include "Adaptee.h"

namespace DesignPattern {
namespace AdapterPattern {

class Adaptee;
class Adapter1: public Target {
public:
	Adapter1();
	Adapter1(Adaptee* adaptee);
	~Adapter1();
	virtual void Request();
private:
	Adaptee* _adaptee;
};

} /* namespace AdapterPattern */
} /* namespace DesignPattern */
