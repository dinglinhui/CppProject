#if 1
#ifndef SRC_CRTP_HPP_
#define SRC_CRTP_HPP_
/*
 The Curiously Recurring Template Pattern (CRTP)
 The CRTP is a pattern which has many applications, but its main feature is that you can define a class
 which derives from a template class where the class being defined is itself a template type parameter.
 */
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

    virtual Vehicle * clone() const override
    {
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

    virtual void describe() const override
    {
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

    virtual void describe() const override
    {
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

    virtual void describe() const override
    {
        std::cout << "I am a fighter plane" << std::endl;
    }
};

} /* namespace application */

//-----------------------------------------------------------------------------
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

//-----------------------------------------------------------------------------

template<class Derived>
class Equality {
};

template<class Derived>
bool operator ==(Equality<Derived> const& op1, Equality<Derived> const & op2) {
    Derived const& d1 = static_cast<Derived const&>(op1);
    Derived const& d2 = static_cast<Derived const&>(op2);
    return !(d1 < d2) && !(d2 < d1);
}

template<class Derived>
bool operator !=(Equality<Derived> const& op1, Equality<Derived> const & op2) {
    Derived const& d1 = static_cast<Derived const&>(op1);
    Derived const& d2 = static_cast<Derived const&>(op2);
    return !(op1 == op2);
}

struct Apple: public Equality<Apple> {
    Apple(int s) :
            size { s } {
    }
    int size;
};

bool operator <(Apple const& a1, Apple const& a2) {
    return a1.size < a2.size;
}

struct Man: public Equality<Man> {
    Man(std::string n) :
            name { n } {
    }
    std::string name;
};

bool operator <(Man const& m1, Man const& m2) {
    return m1.name < m2.name;
}

void test_crtp() {

    std::cout << std::boolalpha << std::endl;

    Apple apple1 { 5 };
    Apple apple2 { 10 };
    std::cout << "apple1 == apple2: " << (apple1 == apple2) << std::endl;

    Man man1 { "grimm" };
    Man man2 { "jaud" };
    std::cout << "man1 != man2: " << (man1 != man2) << std::endl;

    std::cout << std::endl;
}

#endif

#endif /* SRC_CRTP_HPP_ */
