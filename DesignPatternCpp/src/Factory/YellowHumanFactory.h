//YellowHumanFactory.h

#pragma once

#include "ihumanfactory.h"

namespace DesignPattern {
namespace FactoryPattern {

class CYellowHumanFactory: public IHumanFactory {
public:
	CYellowHumanFactory(void);
	~CYellowHumanFactory(void);
	virtual IHuman * CreateHuman(void);
};

} /* namespace FactoryPattern */
} /* namespace DesignPattern */
