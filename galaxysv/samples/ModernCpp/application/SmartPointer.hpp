#ifndef SRC_APPLICATION_SMARTPOINTER_HPP_
#define SRC_APPLICATION_SMARTPOINTER_HPP_

#include <iostream>
#include <memory>

#include "clone.hpp"

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
    std::vector<std::unique_ptr<Vehicle>> vehicles;
    vehicles.emplace_back(new Car);
    vehicles.emplace_back(new Plane);
    vehicles.emplace_back(new FighterPlane);

    for (auto& vehicle : vehicles)
        vehicle->describe();

    std::unique_ptr<Vehicle> vehicleUnknown(vehicles[0]->clone());
    auto vehicleUnknown_1 = std::shared_ptr<Vehicle>(vehicles[1]->clone());
    auto vehicleUnknown_2 = std::shared_ptr<Vehicle>(vehicles[2]->clone());
    vehicleUnknown_2 = std::move(vehicleUnknown);
    vehicleUnknown_1 = vehicleUnknown_2;

    if (vehicleUnknown != nullptr)
        vehicleUnknown->describe();
    vehicleUnknown_1->describe();
    vehicleUnknown_2->describe();

    std::shared_ptr<Man> man(new Man());
    std::shared_ptr<Woman> woman(new Woman());
    if (man && woman) {
        man->marriage(woman);
        woman->marriage(man);
    }
}

}/* namespace application */
#endif /* SRC_APPLICATION_SMARTPOINTER_HPP_ */
