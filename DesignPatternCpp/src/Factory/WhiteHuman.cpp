//WhiteHuman.cpp

#include "WhiteHuman.h"
#include <iostream>

namespace DesignPattern {
namespace FactoryPattern {

CWhiteHuman::CWhiteHuman(void) {
}

CWhiteHuman::~CWhiteHuman(void) {
}

void CWhiteHuman::Cry() {
	std::cout << "CWhiteHuman::Cry" << std::endl;
}

void CWhiteHuman::Laugh() {
	std::cout << "CWhiteHuman::Laugh" << std::endl;
}

void CWhiteHuman::Talk() {
	std::cout << "CWhiteHuman::Talk" << std::endl;
}

} /* namespace FactoryPattern */
} /* namespace DesignPattern */
