#pragma once

#include "Target.h"
#include "Adaptee.h"

namespace DesignPattern {
namespace AdapterPattern {

class Adapter: public Target, private Adaptee {
public:
	Adapter();
	virtual ~Adapter();
	virtual void Request();
};

} /* namespace AdapterPattern */
} /* namespace DesignPattern */
