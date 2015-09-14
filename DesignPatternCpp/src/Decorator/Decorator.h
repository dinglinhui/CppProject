#pragma once

#include "Component.h"
namespace DesignPattern
{
namespace DecoratorPattern
{
class Component;
class Decorator: public Component
{
public:
	Decorator(Component* com);
	void SetComponent(Component* com);
	virtual ~Decorator();
	virtual void Operation();
protected:
	Component* _com;
};

} /* namespace DecoratorPattern */
} /* namespace DesignPattern */
