#ifndef SRC_APPLICATION_CLONE_HPP_
#define SRC_APPLICATION_CLONE_HPP_

#include <iostream>

namespace application {

class Vehicle {
protected:
    int fuelCapacity = 0;

public:
    Vehicle() {
    }

    Vehicle(int fuelCapacity) :
            fuelCapacity(fuelCapacity) {
    }

    Vehicle(const Vehicle& copy) :
            fuelCapacity(copy.fuelCapacity) {
//        std::cout << "Vehicle copy constructor" << std::endl;
    }

    virtual ~Vehicle() {
//        std::cout << "Vehicle destructor" << std::endl;
    }

    virtual Vehicle * clone() const = 0;
    virtual void describe() const = 0;
};

template<typename Base, typename Derived>
class VehicleCloneable: public Base {
public:
    using Base::Base;

    virtual ~VehicleCloneable() {
        //std::cout << "~VehicleCloneable" << std::endl;
    }

    virtual Vehicle * clone() const override {
        return new Derived(static_cast<Derived const &>(*this));
    }
};

class Car: public VehicleCloneable<Vehicle, Car> {
private:
    typedef VehicleCloneable<Vehicle, Car> BaseClass;
    std::string brand;
public:
    Car() {
    }

    Car(int fuelCapacity) :
            BaseClass(fuelCapacity) {
    }

    Car(const Car & copy) :
            BaseClass(copy), brand(copy.brand) {
//        std::cout << "car copy constructor" << std::endl;
    }

    virtual ~Car() {
        std::cout << "Car destructor" << std::endl;
    }

    virtual void describe() const override {
        std::cout << "I am a car" << std::endl;
    }
};

class Plane: public VehicleCloneable<Vehicle, Plane> {
private:
    typedef VehicleCloneable<Vehicle, Plane> BaseClass;

protected:
    int wingSpan = 0;

public:
    Plane() {
    }

    Plane(int fuelCapacity, int wingSpan) :
            BaseClass(fuelCapacity), wingSpan(wingSpan) {
    }

    Plane(const Plane& copy) :
            BaseClass(copy), wingSpan(copy.wingSpan) {
//        std::cout << "plane copy constructor" << std::endl;
    }

    virtual ~Plane() {
        std::cout << "plane destructor" << std::endl;
    }

    virtual void describe() const override {
        std::cout << "I am a plane" << std::endl;
    }
};

class FighterPlane: public VehicleCloneable<Plane, FighterPlane> {
private:
    typedef VehicleCloneable<Plane, FighterPlane> BaseClass;

protected:
    int numberOfBombs = 0;

public:
    FighterPlane() {
    }

    FighterPlane(int fuelCapacity, int wingSpan, int numberOfBombs) :
            BaseClass(fuelCapacity, wingSpan), numberOfBombs(numberOfBombs) {
    }

    FighterPlane(const FighterPlane& copy) :
            BaseClass(copy), numberOfBombs(copy.numberOfBombs) {
//        std::cout << "fighter plane copy constructor" << std::endl;
    }

    virtual ~FighterPlane() {
        std::cout << "fighter plane destructor" << std::endl;
    }

    virtual void describe() const override {
        std::cout << "I am a fighter plane" << std::endl;
    }
};

} /* namespace basic */

#if 1
#define BASE_CLONEABLE(Type) \
    virtual Type *clone() const = 0;

#define CLONEABLE(Type) \
    virtual Type *clone() const { return new Type(*this); }

class Vehicle {
public:
    BASE_CLONEABLE(Vehicle)

    virtual ~Vehicle() {
    }

    virtual void describe() const = 0;
};

class Car: public Vehicle {
public:
    CLONEABLE(Car)

    virtual void describe() const {
        std::cout << "I am a car" << std::endl;
    }
};

class Plane: public Vehicle {
public:
    CLONEABLE(Plane)

    virtual void describe() const {
        std::cout << "I am a plane" << std::endl;
    }
};

class FighterPlane: public Plane {
public:
    CLONEABLE(FighterPlane)

    virtual void describe() const {
        std::cout << "I am a fighter plane" << std::endl;
    }
};
#endif

#endif /* SRC_APPLICATION_CLONE_HPP_ */
