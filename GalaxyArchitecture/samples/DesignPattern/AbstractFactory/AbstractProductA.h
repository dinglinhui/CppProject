#pragma once

#include <iostream>
namespace DesignPattern {
namespace AbstractFactoryPattern {
//抽象基类AbstractProductA，代表A类产品的抽象
class AbstractProductA {
public:
	virtual ~AbstractProductA()=0;
	virtual void operation()=0;
protected:
	AbstractProductA(); //屏蔽构造函数
};

} /* namespace AbstractFactoryPattern */
} /* namespace DesignPattern */
