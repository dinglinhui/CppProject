#pragma once

#include <string>
#include "Colleage.h"

namespace DesignPattern {
namespace MediatorPattern {

class Colleage;
class Mediator {
public:
	virtual ~Mediator();
	virtual void SendMsg(std::string msg, Colleage* p) = 0;
protected:
	Mediator();
};

} /* namespace MediatorPattern */
} /* namespace DesignPattern */
