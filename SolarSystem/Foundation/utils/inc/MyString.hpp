#ifndef _MyString_H_
#define _MyString_H_

#include <iostream>
using namespace std;

class MyString {
public:
	MyString(const char *str = "");
	MyString(MyString &&other);
	MyString(const MyString &other);
	MyString &operator=(MyString &&other);
	MyString &operator=(const MyString &other);
	MyString &operator=(const char *str);

	bool operator!() const;
	char &operator[](unsigned int index);
	const char &operator[](unsigned int index) const;

	friend MyString operator+(const MyString &s1, const MyString &s2);
	MyString &operator+=(const MyString &other);

	friend ostream &operator<<(ostream &os, const MyString &str);
	friend istream &operator>>(istream &is, MyString &str);

	~MyString(void);

	void Display() const;
	int Length() const;
	bool IsEmpty() const;

private:
	MyString &Assign(const char *str);
	char *AllocAndCpy(const char *str);
	char *_str;
};

#endif // _MyString_H_
