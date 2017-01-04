//BlackHuman.h

#pragma once

#include "IHuman.h"

namespace DesignPattern {
namespace FactoryPattern {

class CBlackHuman: public IHuman {
public:
	CBlackHuman(void);
	~CBlackHuman(void);
	void Laugh();
	void Cry();
	void Talk();
};

} /* namespace FactoryPattern */
} /* namespace DesignPattern */
