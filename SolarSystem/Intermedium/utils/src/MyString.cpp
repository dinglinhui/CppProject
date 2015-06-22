#include "MyString.hpp"
#include <cstring>
#include <cassert>

namespace utils {
MyString::MyString(const char *str) {
	_str = AllocAndCpy(str);
}

MyString::MyString(MyString&& other) {
	_str = std::move(other._str);
	other._str = nullptr;
}

MyString::MyString(const MyString &other) {
	_str = AllocAndCpy(other._str);
}

MyString& MyString::operator=(MyString&& other) {
	assert(this != &other);

	delete _str;
	_str = std::move(other._str);
	other._str = nullptr;
	return *this;
}

MyString &MyString::operator=(const MyString &other) {
	if (this == &other)
		return *this;

	return Assign(other._str);
}

MyString &MyString::operator=(const char *str) {
	return Assign(str);
}

MyString &MyString::Assign(const char *str) {
	delete[] _str;
	_str = AllocAndCpy(str);
	return *this;
}

bool MyString::operator!() const {
	return strlen(_str) != 0;
}

char &MyString::operator[](unsigned int index) {
	//return _str[index];
	//non const 版本调用 const版本
	return const_cast<char &>(static_cast<const MyString &>(*this)[index]);
}

const char &MyString::operator[](unsigned int index) const {
	return _str[index];
}

MyString::~MyString() {
	delete[] _str;
}

char *MyString::AllocAndCpy(const char *str) {
	int len = strlen(str) + 1;
	char *newstr = new char[len];
	memset(newstr, 0, len);
	strcpy(newstr, str);

	return newstr;
}

void MyString::Display() const {
	std::cout << _str << std::endl;
}

int MyString::Length() const {
	return strlen(_str);
}

bool MyString::IsEmpty() const {
	return Length() == 0;
}

MyString operator+(const MyString &s1, const MyString &s2) {
	//int len = strlen(s1._str) + strlen(s2._str) + 1;
	//char* newstr = new char[len];
	//memset(newstr, 0, len);
	//strcpy(newstr, s1._str);
	//strcat(newstr, s2._str);
	//
	//MyString tmp(newstr);
	//delete newstr;
	MyString str = s1;
	str += s2;
	return str;
}

MyString &MyString::operator+=(const MyString &other) {
	int len = strlen(_str) + strlen(other._str) + 1;
	char *newstr = new char[len];
	memset(newstr, 0, len);
	strcpy(newstr, _str);
	strcat(newstr, other._str);

	delete[] _str;

	_str = newstr;
	return *this;
}

std::ostream &operator<<(std::ostream &os, const MyString &str) {
	os << str._str;
	return os;
}

std::istream &operator>>(std::istream &is, MyString &str) {
	char tmp[1024];
	std::cin >> tmp;
	str = tmp;
	return is;
}
}
