//IHumanFactory.h

#pragma once

#include "IHuman.h"

namespace DesignPattern {
namespace FactoryPattern {

class IHumanFactory {
public:
	IHumanFactory(void) {
	}
	virtual ~IHumanFactory(void) {
	}
	virtual IHuman * CreateHuman() = 0;
};

} /* namespace FactoryPattern */
} /* namespace DesignPattern */
