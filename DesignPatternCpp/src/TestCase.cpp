/*
 * TestCase.cpp
 *
 *  Created on: Nov 11, 2014
 *      Author: dlh
 */

#include "TestCase.h"

#include "./AbstractFactory/AbstractFactory.h"
#include "./AbstractFactory/ConcreteFactory1.h"
#include "./AbstractFactory/ConcreteFactory2.h"
#include "./AbstractFactory/AbstractProductA.h"
#include "./Adapter/Target.h"
#include "./Adapter/Adaptee.h"
#include "./Adapter/Adapter.h"
#include "./Adapter/Adapter1.h"
#include "./Bridge/Abstraction.h"
#include "./Bridge/AbstractionImplement.h"
#include "./Bridge/RefinedAbstractionA.h"
#include "./Bridge/RefinedAbstractionB.h"
#include "./Bridge/ConcreteAbstractionImplementA.h"
#include "./Bridge/ConcreteAbstractionImplementB.h"
#include "./Builder/Director.h"
#include "./Builder/ConcreteBuilder1.h"
#include "./Builder/ConcreteBuilder2.h"
#include "./Command/Receiver.h"
#include "./Command/Invoker.h"
#include "./Command/Command.h"
#include "./Command/ConcreteCommand.h"
#include "./Composite/Composite.h"
#include "./Composite/Leaf.h"
#include "./Composite/Component.h"
#include "./Composite/Company.hpp"
#include "./Decorator/Decorator.h"
#include "./Decorator/DecoratorOnlyOne.h"
#include "./Decorator/ConcreteComponent.h"
#include "./Decorator/ConcreteDecoratorA.h"
#include "./Decorator/ConcreteDecoratorB.h"
#include "./Decorator/ConcreteDecoratorC.h"
#include "./Decorator/ConcreteDecoratorD.h"
#include "./Facade/Facade.h"
#include "./Facade/Subsystem1.h"
#include "./Facade/Subsystem2.h"
#include "./Factory/IHuman.h"
#include "./Factory/YellowHuman.h"
#include "./Factory/WhiteHuman.h"
#include "./Factory/BlackHuman.h"
#include "./Factory/IHumanFactory.h"
#include "./Factory/YellowHumanFactory.h"
#include "./Factory/WhiteHumanFactory.h"
#include "./Factory/BlackHumanFactory.h"
#include "./Flyweight/Flyweight.h"
#include "./Flyweight/FlyweightFactory.h"
#include "./Memento/Originator.h"
#include "./Memento/Caretaker.h"
#include "./Mediator/Mediator.h"
#include "./Mediator/ConcreteMediator.h"
#include "./Mediator/Colleage.h"
#include "./Mediator/ConcreteColleageA.h"
#include "./Mediator/ConcreteColleageB.h"
#include "./Observer/Subject.h"
#include "./Observer/Observer.h"
#include "./Observer/ConcreteObserverA.h"
#include "./Observer/ConcreteObserverB.h"
#include "./Observer/ConcreteSubjectA.h"
#include "./Observer/ConcreteSubjectB.h"
#include "./Prototype/Prototype.h"
#include "./Prototype/ConcretePrototype1.h"
#include "./Prototype/ConcretePrototype2.h"
#include "./Proxy/Proxy.h"
#include "./Singleton/Singleton.h"
#include "./State/State.h"
#include "./Strategy/Strategy.h"
#include "./TemplateMethod/TemplateMethod.h"
#include "Flyweight/CharacterFactory.h"

namespace DesignPattern {

TestCase::TestCase() {
}

TestCase::~TestCase() {
}

void TestCase::case_abstract_factory() {
	DesignPattern::AbstractFactoryPattern::AbstractFactory* fa1 =
			new DesignPattern::AbstractFactoryPattern::ConcreteFactory1 { };
	DesignPattern::AbstractFactoryPattern::AbstractProductA* a1 =
			fa1->CreateProductA();
	DesignPattern::AbstractFactoryPattern::AbstractProductB* b1 =
			fa1->CreateProductB();

	std::cout << std::endl;
	DesignPattern::AbstractFactoryPattern::AbstractFactory* fa2 =
			new DesignPattern::AbstractFactoryPattern::ConcreteFactory2 { };
	DesignPattern::AbstractFactoryPattern::AbstractProductA* a2 =
			fa2->CreateProductA();
	DesignPattern::AbstractFactoryPattern::AbstractProductB* b2 =
			fa2->CreateProductB();

	std::cout << std::endl;
	delete a1, a1 = nullptr;
	delete b1, b1 = nullptr;
	delete fa1, fa1 = nullptr;

	std::cout << std::endl;
	delete a2, a2 = nullptr;
	delete b2, b2 = nullptr;
	delete fa2, fa2 = nullptr;
}

void TestCase::case_adapter() {
	DesignPattern::AdapterPattern::Target* pTarget =
			new DesignPattern::AdapterPattern::Adapter { };
	pTarget->Request();

	DesignPattern::AdapterPattern::Adaptee* ade =
			new DesignPattern::AdapterPattern::Adaptee { };
	DesignPattern::AdapterPattern::Target* pTarget1 =
			new DesignPattern::AdapterPattern::Adapter1 { ade };
	pTarget1->Request();

	DesignPattern::AdapterPattern::Target* pTarget2 =
			new DesignPattern::AdapterPattern::Adapter1 { };
	pTarget2->Request();
}

void TestCase::case_bridge() {
	DesignPattern::BridgePattern::AbstractionImplement* imp =
			new DesignPattern::BridgePattern::ConcreteAbstractionImplementA { };
	DesignPattern::BridgePattern::Abstraction* abs =
			new DesignPattern::BridgePattern::RefinedAbstractionA { imp };
	abs->Operation();

	cout << "-----------------------------------------" << endl;

	DesignPattern::BridgePattern::AbstractionImplement* imp1 =
			new DesignPattern::BridgePattern::ConcreteAbstractionImplementB { };
	DesignPattern::BridgePattern::Abstraction* abs1 =
			new DesignPattern::BridgePattern::RefinedAbstractionA { imp1 };
	abs1->Operation();

	cout << "-----------------------------------------" << endl;

	DesignPattern::BridgePattern::AbstractionImplement* imp2 =
			new DesignPattern::BridgePattern::ConcreteAbstractionImplementA { };
	DesignPattern::BridgePattern::Abstraction* abs2 =
			new DesignPattern::BridgePattern::RefinedAbstractionB { imp2 };
	abs2->Operation();

	cout << "-----------------------------------------" << endl;

	DesignPattern::BridgePattern::AbstractionImplement* imp3 =
			new DesignPattern::BridgePattern::ConcreteAbstractionImplementB { };
	DesignPattern::BridgePattern::Abstraction* abs3 =
			new DesignPattern::BridgePattern::RefinedAbstractionB { imp3 };
	abs3->Operation();

	cout << endl;
}

void TestCase::case_builder() {
	DesignPattern::BuilderPattern::Director* pDirector =
			new DesignPattern::BuilderPattern::Director {
					new DesignPattern::BuilderPattern::ConcreteBuilder1 { } };
	pDirector->Construct();

	DesignPattern::BuilderPattern::Director* pDirector1 =
			new DesignPattern::BuilderPattern::Director {
					new DesignPattern::BuilderPattern::ConcreteBuilder2 { } };
	pDirector1->Construct();
}

void TestCase::case_command() {
	DesignPattern::CommandPattern::Receiver* pRev =
			new DesignPattern::CommandPattern::Receiver { };
	DesignPattern::CommandPattern::Command* pCmd =
			new DesignPattern::CommandPattern::ConcreteCommand { pRev };

	DesignPattern::CommandPattern::Invoker* pInv =
			new DesignPattern::CommandPattern::Invoker { pCmd };
	pInv->Invoke();
}

void TestCase::case_composite() {
	DesignPattern::CompositePattern::Composite* pRoot =
			new DesignPattern::CompositePattern::Composite { };

	pRoot->Add(new DesignPattern::CompositePattern::Leaf { });

	DesignPattern::CompositePattern::Leaf* pLeaf1 =
			new DesignPattern::CompositePattern::Leaf { };
	DesignPattern::CompositePattern::Leaf* pLeaf2 =
			new DesignPattern::CompositePattern::Leaf { };

	pLeaf1->Add(pLeaf2);
	pLeaf1->Remove(pLeaf2);
	pLeaf1->Operation();

	DesignPattern::CompositePattern::Composite* pCom =
			new DesignPattern::CompositePattern::Composite { };
	pCom->Add(pLeaf1);
	pCom->Add(pLeaf2);
	pCom->Operation();

	pRoot->Add(pCom);

	pRoot->Operation();

	DesignPattern::CompositePattern::Component* cp = pCom->GetChild(0);
	cp->Operation();

	pCom->Remove(pLeaf1);

	//--------------------------------------------------
	DesignPattern::CompositePattern::Company *root =
			new DesignPattern::CompositePattern::ConcreteCompany { "总公司" };
	DesignPattern::CompositePattern::Company *leaf1 =
			new DesignPattern::CompositePattern::FinanceDepartment { "财务部" };
	DesignPattern::CompositePattern::Company *leaf2 =
			new DesignPattern::CompositePattern::HRDepartment { "人力资源部" };
	root->Add(leaf1);
	root->Add(leaf2);

	//分公司A
	DesignPattern::CompositePattern::Company *mid1 =
			new DesignPattern::CompositePattern::ConcreteCompany { "分公司A" };
	DesignPattern::CompositePattern::Company *leaf3 =
			new DesignPattern::CompositePattern::FinanceDepartment { "财务部" };
	DesignPattern::CompositePattern::Company *leaf4 =
			new DesignPattern::CompositePattern::HRDepartment { "人力资源部" };
	mid1->Add(leaf3);
	mid1->Add(leaf4);
	root->Add(mid1);
	//分公司B
	DesignPattern::CompositePattern::Company *mid2 =
			new DesignPattern::CompositePattern::ConcreteCompany { "分公司B" };
	DesignPattern::CompositePattern::FinanceDepartment *leaf5 =
			new DesignPattern::CompositePattern::FinanceDepartment { "财务部" };
	DesignPattern::CompositePattern::HRDepartment *leaf6 =
			new DesignPattern::CompositePattern::HRDepartment { "人力资源部" };
	mid2->Add(leaf5);
	mid2->Add(leaf6);
	root->Add(mid2);
	root->Show(0);

	delete leaf1;
	delete leaf2;
	delete leaf3;
	delete leaf4;
	delete leaf5;
	delete leaf6;
	delete mid1;
	delete mid2;
	delete root;

}

void TestCase::case_decorator() {
	DesignPattern::DecoratorPattern::Component* pCom =
			new DesignPattern::DecoratorPattern::ConcreteComponent { };
	DesignPattern::DecoratorPattern::Decorator* pDec = nullptr;
	pDec = new DesignPattern::DecoratorPattern::ConcreteDecoratorA { pCom };
	pDec = new DesignPattern::DecoratorPattern::ConcreteDecoratorB { pDec };
	pDec = new DesignPattern::DecoratorPattern::ConcreteDecoratorC { pDec };
	pDec = new DesignPattern::DecoratorPattern::ConcreteDecoratorD { pDec };
	pDec->Operation();

	cout << "-------------------------------" << endl;

	DesignPattern::DecoratorPattern::Component* pCom1 =
			new DesignPattern::DecoratorPattern::ConcreteComponent { };
	DesignPattern::DecoratorPattern::DecoratorOnlyOne* pDec1 =
			new DesignPattern::DecoratorPattern::DecoratorOnlyOne { pCom1 };
	pDec1->Operation();

	cout << "-------------------------------" << endl;

	delete pDec;
	cout << "-------------------------------" << endl;

	delete pDec1;
}

void TestCase::case_facade() {
	DesignPattern::FacadePattern::Facade* pFacade =
			new DesignPattern::FacadePattern::Facade { };
	pFacade->OperationWrapper();
}

void TestCase::DoFactoryMethod1() {
	DesignPattern::FactoryPattern::IHumanFactory *pHumanFactory =
			new DesignPattern::FactoryPattern::CYellowHumanFactory { };
	DesignPattern::FactoryPattern::IHuman *pHuman =
			pHumanFactory->CreateHuman();
	pHuman->Cry();
	pHuman->Laugh();
	pHuman->Talk();
	delete pHuman;
	delete pHumanFactory;
}
void TestCase::DoFactoryMethod2() {
	DesignPattern::FactoryPattern::IHumanFactory *pHumanFactory =
			new DesignPattern::FactoryPattern::CWhiteHumanFactory { };
	DesignPattern::FactoryPattern::IHuman *pHuman =
			pHumanFactory->CreateHuman();
	pHuman->Cry();
	pHuman->Laugh();
	pHuman->Talk();
	delete pHuman;
	delete pHumanFactory;
}

void TestCase::DoFactoryMethod3() {
	DesignPattern::FactoryPattern::IHumanFactory *pHumanFactory =
			new DesignPattern::FactoryPattern::CBlackHumanFactory { };
	DesignPattern::FactoryPattern::IHuman *pHuman =
			pHumanFactory->CreateHuman();
	pHuman->Cry();
	pHuman->Laugh();
	pHuman->Talk();
	delete pHuman;
	delete pHumanFactory;
}

void TestCase::case_factory() {
	TestCase::DoFactoryMethod1();
	TestCase::DoFactoryMethod2();
	TestCase::DoFactoryMethod3();
}

void TestCase::case_flyweight() {
	std::string extrinsicState = "ext";
	DesignPattern::FlyweightPattern::FlyweightFactory* fc =
			new DesignPattern::FlyweightPattern::FlyweightFactory { };
	DesignPattern::FlyweightPattern::Flyweight* fly = fc->GetFlyweight("hello");
//	DesignPattern::FlyweightPattern::Flyweight* fly1 = fc->GetFlyweight("hello");
	fly->Operation(extrinsicState);
	fc->GetFlyweightCount();

	{
		int ascent { 70 };
		int descent { 0 };
		int height { 100 };
		int width { 120 };
		int pointSize { 10 };

		std::string test = "AABCDDEFGHI";
		std::string::iterator it = test.begin();
		DesignPattern::FlyweightPattern::CharacterFactory* pcF =
				new DesignPattern::FlyweightPattern::CharacterFactory { };
		for (; it != test.end(); it++) {
			pointSize++;

			char c { *it };
			DesignPattern::FlyweightPattern::Character* charc =
					pcF->GetCharacter(c);
			charc->Display(width, height, ascent, descent, pointSize);
		}
		std::vector<DesignPattern::FlyweightPattern::Character*>::size_type sizeChar =
				pcF->GetCount();
		std::cout << "count:" << sizeChar << std::endl;
	}
}

void TestCase::case_mediator() {
	DesignPattern::MediatorPattern::ConcreteMediator* pMediator =
			new DesignPattern::MediatorPattern::ConcreteMediator { };

	DesignPattern::MediatorPattern::Colleage* cp1 =
			new DesignPattern::MediatorPattern::ConcreteColleageA { pMediator };
	DesignPattern::MediatorPattern::Colleage* cp2 =
			new DesignPattern::MediatorPattern::ConcreteColleageB { pMediator };

	pMediator->SetColleageA(cp1);
	pMediator->SetColleageB(cp2);

	cp1->SendMsg("xxx");
	cp2->SendMsg("ooo");
}

void TestCase::case_memento() {
	DesignPattern::MementoPattern::Originator* o =
			new DesignPattern::MementoPattern::Originator { "Old" };
	o->show();

	DesignPattern::MementoPattern::Caretaker* pTaker =
			new DesignPattern::MementoPattern::Caretaker { };
	pTaker->SetMemento(o->CreateMemento());

	o->SetState("New");
	o->show();

	o->RestoreToMemento(pTaker->GetMemento());
	o->show();
}

void TestCase::case_observer() {
	DesignPattern::ObserverPattern::Observer* op1 =
			new DesignPattern::ObserverPattern::ConcreteObserverA { };
	DesignPattern::ObserverPattern::Observer* op2 =
			new DesignPattern::ObserverPattern::ConcreteObserverB { };
	DesignPattern::ObserverPattern::Observer* op3 =
			new DesignPattern::ObserverPattern::ConcreteObserverA { };

	DesignPattern::ObserverPattern::Subject* pSubject =
			new DesignPattern::ObserverPattern::ConcreteSubjectA { };
	pSubject->Attach(op1);
	pSubject->Attach(op2);
	pSubject->Attach(op3);
	pSubject->SetState("old");
	pSubject->Notify();

	std::cout << "-------------------------------------" << std::endl;
	pSubject->SetState("new");
	pSubject->Detach(op3);
	pSubject->Notify();
}

void TestCase::case_prototype() {
	DesignPattern::PrototypePattern::Prototype* p1 =
			new DesignPattern::PrototypePattern::ConcretePrototype1 { };
	DesignPattern::PrototypePattern::Prototype* p2 = p1->Clone();
	std::cout << "------------------------" << std::endl;

	DesignPattern::PrototypePattern::Prototype* p3 =
			new DesignPattern::PrototypePattern::ConcretePrototype2 { };
	DesignPattern::PrototypePattern::Prototype* p4 = p3->Clone();
	std::cout << "------------------------" << std::endl;

	delete p1;
	delete p2;
	std::cout << "------------------------" << std::endl;

	delete p3;
	delete p4;
}

void TestCase::case_proxy() {
	DesignPattern::ProxyPattern::Proxy* proxy =
			new DesignPattern::ProxyPattern::Proxy { };
	proxy->Request();
}

void TestCase::case_singleton() {
//	Singleton* instance = Singleton::getInstance();
	DesignPattern::SingletonPattern::Singleton::getInstance();
	DesignPattern::SingletonPattern::Singleton::destroyInstance();
}

void TestCase::case_state() {
	DesignPattern::StatePattern::State* pState =
			new DesignPattern::StatePattern::ConcreteStateA { };
	DesignPattern::StatePattern::Context* pContext =
			new DesignPattern::StatePattern::Context { pState };
	pContext->Request();
	pContext->Request();
	pContext->Request();
	pContext->Request();
	pContext->Request();
}

void TestCase::case_strategy() {
	DesignPattern::StrategyPattern::Strategy* pStr =
			new DesignPattern::StrategyPattern::ConcreteStrategyA { };
	DesignPattern::StrategyPattern::Context* pcon =
			new DesignPattern::StrategyPattern::Context { pStr };
	pcon->DoAction();

	pStr = new DesignPattern::StrategyPattern::ConcreteStrategyB { };
	pcon = new DesignPattern::StrategyPattern::Context { pStr };
	pcon->DoAction();
}

void TestCase::case_templatemethod() {
	DesignPattern::TemplateMethodPattern::AbstractClass* pAbstract =
			new DesignPattern::TemplateMethodPattern::ConcreteClassA { };
	pAbstract->TemplateMethod();

	pAbstract = new DesignPattern::TemplateMethodPattern::ConcreteClassB { };
	pAbstract->TemplateMethod();
}

} /* namespace DesignPattern */
