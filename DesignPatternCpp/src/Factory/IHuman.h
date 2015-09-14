//IHuman.h

#pragma once

namespace DesignPattern
{
namespace FactoryPattern
{

class IHuman
{
public:
	IHuman(void)
	{
	}
	virtual ~IHuman(void)
	{
	}
	virtual void Laugh() = 0;
	virtual void Cry() = 0;
	virtual void Talk() = 0;
};

} /* namespace FactoryPattern */
} /* namespace DesignPattern */
