//BlackHumanFactory.h

#pragma once

#include "ihumanfactory.h"
namespace DesignPattern {
namespace FactoryPattern {

class CBlackHumanFactory: public IHumanFactory {
public:
	CBlackHumanFactory(void);
	~CBlackHumanFactory(void);
	virtual IHuman * CreateHuman();
};

} /* namespace FactoryPattern */
} /* namespace DesignPattern */
