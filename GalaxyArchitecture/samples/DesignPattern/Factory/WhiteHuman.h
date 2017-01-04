//WhiteHuman.h

#pragma once

#include "ihuman.h"

namespace DesignPattern {
namespace FactoryPattern {

class CWhiteHuman: public IHuman {
public:
	CWhiteHuman(void);
	~CWhiteHuman(void);
	void Laugh();
	void Cry();
	void Talk();
};

} /* namespace FactoryPattern */
} /* namespace DesignPattern */
