//YellowHuman.h

#pragma once

#include "ihuman.h"

namespace DesignPattern {
namespace FactoryPattern {

class CYellowHuman: public IHuman {
public:
	CYellowHuman(void);
	~CYellowHuman(void);

	void Laugh();
	void Cry();
	void Talk();
};

} /* namespace FactoryPattern */
} /* namespace DesignPattern */
