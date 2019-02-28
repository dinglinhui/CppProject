#ifndef _CRTP_HPP_
#define _CRTP_HPP_

#include <iostream>
#include <memory>
#include <vector>

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
	}

	virtual ~Vehicle() {
	}

	virtual std::unique_ptr<Vehicle> clone() const = 0;
	virtual void describe() const = 0;
};

template<typename Base, typename Derived>
class VehicleCloneable: public Base {
public:
	using Base::Base;

	virtual ~VehicleCloneable() {
	}

	virtual std::unique_ptr<Vehicle> clone() const override {
		return std::unique_ptr<Vehicle> { new Derived(
				static_cast<Derived const &>(*this)) };
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
	}

	virtual ~Car() {
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
	}

	virtual ~Plane() {
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
	}

	virtual ~FighterPlane() {
	}

	virtual void describe() const override {
		std::cout << "I am a fighter plane" << std::endl;
	}
};

#if 0
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

struct Human: public Equality<Human> {
	Human(std::string n) :
			name { n } {
	}
	std::string name;
};

bool operator <(Human const& m1, Human const& m2) {
	return m1.name < m2.name;
}

void test_crtp() {
	std::vector<std::unique_ptr<Vehicle>> vehicles;
	vehicles.emplace_back(new Car);
	vehicles.emplace_back(new Plane);
	vehicles.emplace_back(new FighterPlane);

	for (auto& vehicle : vehicles)
		vehicle->describe();

	std::unique_ptr<Vehicle> vehicleUnknown = vehicles[0]->clone();
	auto vehicleUnknown_1 = std::shared_ptr<Vehicle>(vehicles[1]->clone());
	auto vehicleUnknown_2 = std::shared_ptr<Vehicle>(vehicles[2]->clone());
	vehicleUnknown_2 = std::move(vehicleUnknown);
	vehicleUnknown_1 = vehicleUnknown_2;

	if (vehicleUnknown != nullptr)
		vehicleUnknown->describe();
	vehicleUnknown_1->describe();
	vehicleUnknown_2->describe();

	std::cout << std::boolalpha << std::endl;

	Apple apple1 { 5 };
	Apple apple2 { 10 };
	std::cout << "apple1 == apple2: " << (apple1 == apple2) << std::endl;

	Human man1 { "grimm" };
	Human man2 { "jaud" };
	std::cout << "man1 != man2: " << (man1 != man2) << std::endl;

	std::cout << std::endl;
}

} /* namespace basic */
#endif /* _CRTP_HPP_ */
