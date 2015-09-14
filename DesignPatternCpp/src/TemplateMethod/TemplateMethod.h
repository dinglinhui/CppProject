#pragma once

namespace DesignPattern
{
namespace TemplateMethodPattern
{

class AbstractClass
{
protected:
	AbstractClass();
public:
	~AbstractClass();
	void TemplateMethod();
	virtual void PrimitiveOperation1()=0;
	virtual void PrimitiveOperation2()=0;
};

class ConcreteClassA: public AbstractClass
{
public:
	ConcreteClassA();
	~ConcreteClassA();

	virtual void PrimitiveOperation1();
	virtual void PrimitiveOperation2();
private:
};

class ConcreteClassB: public AbstractClass
{
public:
	ConcreteClassB();
	~ConcreteClassB();

	virtual void PrimitiveOperation1();
	virtual void PrimitiveOperation2();
};

} /* namespace TemplateMethodPattern */
} /* namespace DesignPattern */
