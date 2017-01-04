/*
 * Range.cpp
 *
 *  Created on: Sep 12, 2014
 *      Author: xdinlin
 */
#include <iostream>
#include "FakeIter.hpp"
using namespace std;

namespace lots {
class Range {
	typedef long _VType;    //同样，也可以弄个模板出来，但是就不方便用了
public:
	Range(_VType end_v) :
			begin_value_(0), end_value_(end_v), step_value_(1) {
	}
	Range(_VType begin_v, _VType end_v) :
			begin_value_(begin_v), end_value_(end_v), step_value_(1) {
	}
	Range(_VType begin_v, _VType end_v, _VType step_v) :
			begin_value_(begin_v), end_value_(end_v), step_value_(step_v) {
	}
	~Range() {
	}

	FakeIter begin() const {
		return FakeIter(begin_value_, step_value_);
	}

	FakeIter end() const {
		return FakeIter(end_value_, step_value_);
	}
private:
	_VType begin_value_ { 0 };
	_VType end_value_ { 0 };
	_VType step_value_ { 1 };

};
/* class Range */

} /* namespace lots */
