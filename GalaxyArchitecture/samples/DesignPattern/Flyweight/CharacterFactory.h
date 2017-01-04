#pragma once

#include <vector>
#include "Character.h"

namespace DesignPattern {
namespace FlyweightPattern {

class Character;
class CharacterFactory {
public:
	CharacterFactory();
	~CharacterFactory();
	Character* GetCharacter(char);
	std::vector<Character*>::size_type GetCount();

private:
	std::vector<Character*> m_vecCharacter;
};

} /* namespace FlyweightPattern */
} /* namespace DesignPattern */
