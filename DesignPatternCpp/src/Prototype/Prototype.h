#pragma once

#include <iostream>
namespace DesignPattern
{
namespace PrototypePattern
{

class Prototype
{
protected:
	Prototype();
public:
	virtual Prototype* Clone() const=0;
	virtual ~Prototype();
};

} /* namespace PrototypePattern */
} /* namespace DesignPattern */
