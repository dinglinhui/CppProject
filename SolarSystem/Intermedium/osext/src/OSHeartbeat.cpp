/*
 * OSHeartbeat.cpp
 *
 *  Created on: Jun 22, 2015
 *      Author: dinglinhui
 */

#include <OSHeartbeat.h>

namespace osext {

OSHeartbeat::OSHeartbeat() :
		pre_value(0), cur_value(0) {
}

OSHeartbeat::~OSHeartbeat() {
}

OSHeartbeat& OSHeartbeat::operator++() {
	this->cur_value++;
	return *this;
}

const OSHeartbeat OSHeartbeat::operator++(int) {
	OSHeartbeat oldValue = *this;
	this->cur_value++;
	return oldValue;
}

OSHeartbeat& OSHeartbeat::operator--() {
	this->pre_value = this->cur_value;
	return *this;
}

const OSHeartbeat OSHeartbeat::operator--(int) {
	OSHeartbeat oldValue = *this;
	this->pre_value = this->cur_value;
	return oldValue;
}

} /* namespace OSExt */
