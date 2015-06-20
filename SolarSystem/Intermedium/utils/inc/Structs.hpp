/*
 * Structs.hpp
 *
 *  Created on: Apr 8, 2015
 *      Author: dinglinhui
 */

#ifndef STRUCTS_HPP_
#define STRUCTS_HPP_

namespace OSUtils {

struct SomeType {
	SomeType() = default;
//	SomeType(OtherType value);
};

struct NonCopyable {
	NonCopyable & operator=(const NonCopyable&) = delete;
	NonCopyable(const NonCopyable&) = delete;
	NonCopyable() = default;
};

struct NonNewable {
	void *operator new(std::size_t) = delete;
};

struct NoDouble {
	void f(int i);
	void f(double) = delete;
};

struct OnlyInt {
	void f(int i);
	template<class T> void f(T) = delete;
};

} //namespace OSUtils
#endif /* STRUCTS_HPP_ */
