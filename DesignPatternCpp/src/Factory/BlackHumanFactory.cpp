//BlackHumanFactory.cpp

#include "BlackHumanFactory.h"
#include "BlackHuman.h"

namespace DesignPattern
{
namespace FactoryPattern
{

CBlackHumanFactory::CBlackHumanFactory(void)
{
}

CBlackHumanFactory::~CBlackHumanFactory(void)
{
}

IHuman * CBlackHumanFactory::CreateHuman()
{
    return new CBlackHuman();
}

} /* namespace FactoryPattern */
} /* namespace DesignPattern */
