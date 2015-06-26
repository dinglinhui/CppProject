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

std::ostream &operator<<(std::ostream &os, const OSHeartbeat &heartbeat) {
	os << "previous:" << heartbeat.pre_value << " current:"
			<< heartbeat.cur_value << std::endl;
	return os;
}

std::istream &operator>>(std::istream &is, OSHeartbeat &heartbeat) {
	ULONGLONG tmp;
	std::cin >> tmp;
	heartbeat.cur_value = tmp;
	return is;
}

} /* namespace OSExt */
