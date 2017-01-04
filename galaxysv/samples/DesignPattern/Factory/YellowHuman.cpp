//YellowHuman.cpp

#include "YellowHuman.h"
#include <iostream>

namespace DesignPattern {
namespace FactoryPattern {

CYellowHuman::CYellowHuman(void) {
}

CYellowHuman::~CYellowHuman(void) {
}

void CYellowHuman::Cry() {
	std::cout << "CYellowHuman::Cry" << std::endl;
}

void CYellowHuman::Laugh() {
	std::cout << "CYellowHuman::Laugh" << std::endl;
}

void CYellowHuman::Talk() {
	std::cout << "CYellowHuman::Talk" << std::endl;
}

} /* namespace FactoryPattern */
} /* namespace DesignPattern */
