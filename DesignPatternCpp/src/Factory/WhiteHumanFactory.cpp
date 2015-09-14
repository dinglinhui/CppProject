//WhiteHumanFactory.cpp

#include "WhiteHumanFactory.h"
#include "WhiteHuman.h"

namespace DesignPattern
{
namespace FactoryPattern
{

CWhiteHumanFactory::CWhiteHumanFactory(void)
{
}

CWhiteHumanFactory::~CWhiteHumanFactory(void)
{
}

IHuman * CWhiteHumanFactory::CreateHuman( void )
{
    return new CWhiteHuman();
}

} /* namespace FactoryPattern */
} /* namespace DesignPattern */
