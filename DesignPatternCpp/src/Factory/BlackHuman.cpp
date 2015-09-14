//BlackHuman.cpp

#include "BlackHuman.h"
#include <iostream>

namespace DesignPattern
{
namespace FactoryPattern
{

CBlackHuman::CBlackHuman(void)
{
}
CBlackHuman::~CBlackHuman(void)
{
}
void CBlackHuman::Cry()
{
    std::cout << " CBlackHuman::Cry" << std::endl;
}
void CBlackHuman::Laugh()
{
	std::cout << "CBlackHuman::Laugh" << std::endl;
}
void CBlackHuman::Talk()
{
	std::cout << "CBlackHuman::Talk" << std::endl;
}

} /* namespace FactoryPattern */
} /* namespace DesignPattern */
