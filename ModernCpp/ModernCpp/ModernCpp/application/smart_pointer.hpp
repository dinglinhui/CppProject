#ifndef _SMARTPOINTER_HPP_
#define _SMARTPOINTER_HPP_

#include <iostream>
#include <memory>

namespace application {

class Woman;
class Man {
private:
    std::weak_ptr<Woman> _wife;
public:
    void marriage(std::shared_ptr<Woman> woman) {
        _wife = woman;
    }

    void doSomthing() {
        std::shared_ptr<Woman> wife = _wife.lock();
        if (wife != nullptr) {

        }
    }

    ~Man() {
        std::cout << "kill man\n";
    }
};

class Woman {
private:
    std::shared_ptr<Man> _husband;
public:
    void marriage(std::shared_ptr<Man> man) {
        _husband = man;
    }

    ~Woman() {
        std::cout << "kill woman\n";
    }
};

void test_smartpointer() {
    std::shared_ptr<Man> man(new Man());
    std::shared_ptr<Woman> woman(new Woman());
    if (man && woman) {
        man->marriage(woman);
        woman->marriage(man);
    }
}

}/* namespace application */
#endif /* _SMARTPOINTER_HPP_ */
