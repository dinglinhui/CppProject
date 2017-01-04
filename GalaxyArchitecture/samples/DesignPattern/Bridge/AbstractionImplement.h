#pragma once

#include <iostream>
namespace DesignPattern {
namespace BridgePattern {

class AbstractionImplement {
public:
	virtual void Operation()=0;
	virtual ~AbstractionImplement();
protected:
	AbstractionImplement();
};

}
}

