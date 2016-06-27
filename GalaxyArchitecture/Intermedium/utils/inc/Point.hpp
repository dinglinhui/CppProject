/*
 * point.hpp

 *
 *  Created on: June 3, 2015
 *      Author: dlh
 */

#ifndef POINT_HPP_
#define POINT_HPP_

#include <iostream>
#include <vector>
#include <cassert>
#include <cstdlib>
#include <string>
#include <stdexcept>

namespace utils {
template<class Type, int Dim>
class Point {
private:
	Type _coords[Dim];

public:
	Point() = delete;
	Point(Type coords[Dim]);
	Point(const Type& v);
	Point(Type&& v);
	Point(const Point& other);
	Point(Point&& other);
	Point& operator=(const Point &other);
	Point& operator=(Point &&other);

	~Point();

	Type& operator[](int index) {
		if (index >= Dim || index < 0) {
			throw std::out_of_range("index is out of dimension range");
		}
		return _coords[index];
	}

	const Type& operator[](int index) const {
		if (index >= Dim || index < 0) {
			throw std::out_of_range("index is out of dimension range");
		}
		return _coords[index];
	}
};

template<class Type, int Dim>
Point<Type, Dim>::Point(Type coords[Dim]) {
	for (int ix = 0; ix < Dim; ix++)
		_coords[ix] = coords[ix];
}

template<class Type, int Dim>
Point<Type, Dim>::Point(const Type &v) {
}

template<class Type, int Dim>
Point<Type, Dim>::Point(Type &&v) {
}

template<class Type, int Dim>
Point<Type, Dim>::Point(const Point<Type, Dim> &other) {
}

template<class Type, int Dim>
Point<Type, Dim>::Point(Point<Type, Dim> &&other) {
}

template<class Type, int Dim>
Point<Type, Dim>& Point<Type, Dim>::operator=(const Point<Type, Dim> &other) {
	return *this;
}

template<class Type, int Dim>
Point<Type, Dim>& Point<Type, Dim>::operator=(Point<Type, Dim> &&other) {
	return *this;
}

template<class Type, int Dim>
Point<Type, Dim>::~Point() {

}

template<class Type, int Dim>
inline std::ostream& operator<<(std::ostream &os, const Point<Type, Dim> &pt) {
	os << "( ";
	for (int ix = 0; ix < Dim - 1; ix++)
		os << pt[ix] << ", ";
	os << pt[Dim - 1];
	os << " )";
	return os;
}
} //namespace utils

namespace test {
using namespace utils;

template<class Type, int Dim>
void testPoint(Type (&coords)[Dim]) {
	std::cout << "testPoint:" << std::endl;
	Point<Type, Dim> p(coords);
	std::cout << p << std::endl;
}

} //namespace test

#endif //POINT_HPP_
