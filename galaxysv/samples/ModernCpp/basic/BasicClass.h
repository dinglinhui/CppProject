#ifndef SRC_BASIC_BASICCPP_H_
#define SRC_BASIC_BASICCPP_H_
#include <iostream>
#include <cassert>

namespace basic {

class BasicClass {
public:
    BasicClass();
    virtual ~BasicClass();

    void GetMemory(char **p, int num) {
        *p = new char[sizeof(char) * num];
    }

    void GetMemory(char *&p, int num) {
        p = new char[sizeof(char) * num];
    }

    char * GetMemory(int num) {
        char * p = new char[sizeof(char) * num];
        return p;
    }

private:
    BasicClass *ptr = nullptr;
    //BasicCpp obj; //BasicCpp didn't initialize completely until constructor finished .
    BasicClass &ref;
};

class Base;

class Another {
    friend class Base;
public:
    Another() {
    }

    ~Another() {
    }

protected:
    void Operation() {
        std::cout << "Hello, I am here." << std::endl;
    }

};

class Base {
public:
    Base() {
    }

    virtual ~Base() {
    }

    virtual void Operate() {
        a.Operation();
    }

private:
    Another a;
};

class Derived: private Base {
public:
    Derived() {
    }

    ~Derived() {
    }

    void Operate_1() {
        Operate();
    }
};

} /* namespace basic */

#endif /* SRC_BASIC_BASICCPP_H_ */
