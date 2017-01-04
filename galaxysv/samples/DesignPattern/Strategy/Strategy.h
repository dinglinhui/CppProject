#pragma once

namespace DesignPattern {
namespace StrategyPattern {

class Strategy {
public:
	virtual ~Strategy();
	virtual void AlgrithmInterface() = 0;

protected:
	Strategy();
private:
};

class ConcreteStrategyA: public Strategy {
public:
	ConcreteStrategyA();
	virtual ~ConcreteStrategyA();

	virtual void AlgrithmInterface();
};

class ConcreteStrategyB: public Strategy {
public:
	ConcreteStrategyB();
	virtual ~ConcreteStrategyB();

	virtual void AlgrithmInterface();
};

class Context {
public:
	Context(Strategy*);
	virtual ~Context();

	void DoAction();

private:
	Strategy* _strategy;
};

} /* namespace StrategyPattern */
} /* namespace DesignPattern */
