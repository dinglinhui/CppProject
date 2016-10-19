//WhiteHumanFactory.h

#pragma once

#include "ihumanfactory.h"
namespace DesignPattern {
namespace FactoryPattern {

class CWhiteHumanFactory: public IHumanFactory {
public:
	CWhiteHumanFactory(void);
	~CWhiteHumanFactory(void);
	virtual IHuman * CreateHuman(void);
};

} /* namespace FactoryPattern */
} /* namespace DesignPattern */
