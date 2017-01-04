#pragma once

#include <iostream>
namespace DesignPattern {
namespace AbstractFactoryPattern {
//抽象基类AbstractProductB，代表B类产品的抽象
class AbstractProductB {
public:
	virtual ~AbstractProductB()=0;
	virtual void operation()=0;
protected:
	AbstractProductB(); //屏蔽构造函数
};

} /* namespace AbstractFactoryPattern */
} /* namespace DesignPattern */
