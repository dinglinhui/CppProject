#include <cassert>
#include "TestCase.h"

DesignPatternType type = DesignPatternType::OBSERVER;

int main(int argc, const char *argv[]) {
	switch (type) {
	case DesignPatternType::ABSTRACT_FACTORY:
		DesignPattern::TestCase::case_abstract_factory();
		break;
	case DesignPatternType::ADAPTER:
		DesignPattern::TestCase::case_adapter();
		break;
	case DesignPatternType::BRIDGE:
		DesignPattern::TestCase::case_bridge();
		break;
	case DesignPatternType::BUILDER:
		DesignPattern::TestCase::case_builder();
		break;
	case DesignPatternType::COMMAND:
		DesignPattern::TestCase::case_command();
		break;
	case DesignPatternType::COMPOSITE:
		DesignPattern::TestCase::case_composite();
		break;
	case DesignPatternType::DECORATOR:
		DesignPattern::TestCase::case_decorator();
		break;
	case DesignPatternType::FACADE:
		DesignPattern::TestCase::case_facade();
		break;
	case DesignPatternType::FACTORY:
		DesignPattern::TestCase::case_factory();
		break;
	case DesignPatternType::FLYWEIGHT:
		DesignPattern::TestCase::case_flyweight();
		break;
	case DesignPatternType::MEDIATOR:
		DesignPattern::TestCase::case_mediator();
		break;
	case DesignPatternType::MEMENTO:
		DesignPattern::TestCase::case_memento();
		break;
	case DesignPatternType::OBSERVER:
		DesignPattern::TestCase::case_observer();
		break;
	case DesignPatternType::PROTOTYPE:
		DesignPattern::TestCase::case_prototype();
		break;
	case DesignPatternType::PROXY:
		DesignPattern::TestCase::case_proxy();
		break;
	case DesignPatternType::SINGLETON:
		DesignPattern::TestCase::case_singleton();
		break;
	case DesignPatternType::STATE:
		DesignPattern::TestCase::case_state();
		break;
	case DesignPatternType::STRATEGY:
		DesignPattern::TestCase::case_strategy();
		break;
	case DesignPatternType::TEMPLATEMETHOD:
		DesignPattern::TestCase::case_templatemethod();
		break;
	default:
		break;
	}

	return 0;
}

