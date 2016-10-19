#include <iostream>
#include "CharacterFactory.h"
#include "CharacterA.h"

namespace DesignPattern {
namespace FlyweightPattern {

CharacterFactory::CharacterFactory() {
}

CharacterFactory::~CharacterFactory() {
}

Character* CharacterFactory::GetCharacter(char c) {
	std::vector<Character*>::iterator iter = this->m_vecCharacter.begin();
	for (; iter != this->m_vecCharacter.end(); iter++) {
		if ((*iter)->GetSymbol() == c) {
			return *iter;
		}
	}
	Character* pf = new CharacterA(c);
	this->m_vecCharacter.push_back(pf);
	return pf;
}

std::vector<Character*>::size_type CharacterFactory::GetCount() {
	return this->m_vecCharacter.size();
}

} /* namespace FlyweightPattern */
} /* namespace DesignPattern */
