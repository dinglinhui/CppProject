/*
 * components.HPP
 *
 *  Created on: Jun 10, 2015
 *      Author: dlh
 */

#ifndef COMPONENTS_HPP_
#define COMPONENTS_HPP_

#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <list>

class NLComponent { //抽象基类
public:
	virtual std::ostream& print(std::ostream& s) const = 0;
	virtual ~NLComponent() {
	}
};

class TextBlock: public NLComponent {  //子类一
public:
	TextBlock(std::string name) :
			name(name) {
	}
	virtual ~TextBlock();
	virtual std::ostream& print(std::ostream& s) const;
private:
	std::string name;  //名称
};

std::ostream& TextBlock::print(std::ostream& s) const {
	s << "TextBlock : " << name << std::endl;
	return s;
}

TextBlock::~TextBlock() {
	std::cout << "TextBlock Destroy" << std::endl;
}

class Graphic: public NLComponent { //子类二
public:
	Graphic(std::string name) :
			name(name) {
	}
	virtual ~Graphic();
	virtual std::ostream& print(std::ostream& s) const;
private:
	std::string name;   //名称
};

std::ostream& Graphic::print(std::ostream& s) const {
	s << "Graphic : " << name << std::endl;
	return s;
}

Graphic::~Graphic() {
	std::cout << "Graphic Destroy" << std::endl;
}

inline std::ostream& operator<<(std::ostream& s, const NLComponent& c) {
	return c.print(s);
}

class NewLetter {
public:
	NewLetter(std::ifstream &fin);
	virtual ~NewLetter();
	void printAll();
private:
	std::list<NLComponent*> components;
};

NewLetter::NewLetter(std::ifstream &fin) {
	std::string str;
	while (fin >> str)   //根据不同的信息，构造不同的子类对象，即构造函数虚拟化
	{
		if (std::string::npos != str.find("TextBlock")) {
			components.push_back(new TextBlock(str));
		} else if (std::string::npos != str.find("Graphic")) {
			components.push_back(new Graphic(str));
		}
	}
}

NewLetter::~NewLetter() {
	std::for_each(std::begin(components), std::end(components),
			[](NLComponent *component) {
				delete component;
			});
//	for (std::list<NLComponent*>::const_iterator it = components.begin();
//			it != components.end(); ++it) {
//		delete (*it);
//	}
	components.clear();
}

void NewLetter::printAll() {
	std::for_each(std::begin(components), std::end(components),
			[](NLComponent *component) {
				std::cout << *component;
			});
//	for (std::list<NLComponent*>::const_iterator it = components.begin();
//			it != components.end(); ++it) {
//		std::cout << **it;    //直接输出不同子类的信息，即虚拟化非成员函数
//	}
}

int testComponent(int argc, char* argv[]) {
	std::ifstream fin(*++argv);
	NewLetter *letter = new NewLetter(fin);
	letter->printAll();
	delete letter;

	return 0;
}

#endif /* COMPONENTS_HPP_ */
