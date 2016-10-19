//YellowHumanFactory.cpp

#include "YellowHumanFactory.h"
#include "YellowHuman.h"

namespace DesignPattern {
namespace FactoryPattern {

CYellowHumanFactory::CYellowHumanFactory(void) {
}

CYellowHumanFactory::~CYellowHumanFactory(void) {
}

IHuman * CYellowHumanFactory::CreateHuman(void) {
	return new CYellowHuman();
}

} /* namespace FactoryPattern */
} /* namespace DesignPattern */
