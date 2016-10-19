/*
 * TestCase.h
 *
 *  Created on: Nov 11, 2014
 *      Author: dlh
 */

#pragma once

enum class DesignPatternType {
	ABSTRACT_FACTORY,
	ADAPTER,
	BRIDGE,
	BUILDER,
	COMMAND,
	COMPOSITE,
	DECORATOR,
	FACADE,
	FACTORY,
	FLYWEIGHT,
	MEDIATOR,
	MEMENTO,
	OBSERVER,
	PROTOTYPE,
	PROXY,
	SINGLETON,
	STATE,
	STRATEGY,
	TEMPLATEMETHOD
};

namespace DesignPattern {

class TestCase {
private:
	TestCase();
	virtual ~TestCase();
public:
	static void case_abstract_factory();
	static void case_adapter();
	static void case_bridge();
	static void case_builder();
	static void case_command();
	static void case_composite();
	static void case_decorator();
	static void case_facade();
	static void case_factory();
	static void case_flyweight();
	static void case_mediator();
	static void case_memento();
	static void case_observer();
	static void case_prototype();
	static void case_proxy();
	static void case_singleton();
	static void case_state();
	static void case_strategy();
	static void case_templatemethod();

private:
	static void DoFactoryMethod1();
	static void DoFactoryMethod2();
	static void DoFactoryMethod3();
};

} /* namespace DesignPattern */
