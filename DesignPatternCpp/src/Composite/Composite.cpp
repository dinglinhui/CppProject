#include "Composite.h"
#include <iostream>
#include <algorithm>

namespace DesignPattern
{
namespace CompositePattern
{

Composite::Composite()
{
}

Composite::~Composite()
{
}

void Composite::Add(Component* com)
{
	this->m_ComVec.push_back(com);
}

void Composite::Remove(Component* com)
{
	std::vector<Component*>::iterator position = std::find(this->m_ComVec.begin(), this->m_ComVec.end(), com);
	if (position != this->m_ComVec.end()) // == vector.end() means the element was not found
		this->m_ComVec.erase(position);
}

void Composite::Operation()
{
	std::cout << "Composite::Operation" << std::endl;
	std::vector<Component*>::iterator iter = this->m_ComVec.begin();
	for (; iter != this->m_ComVec.end(); iter++)
	{
		(*iter)->Operation();
	}
}

Component* Composite::GetChild(int index)
{
	if (index < 0 || (size_t)index > this->m_ComVec.size())
	{
		return nullptr;
	}
	return this->m_ComVec[index];
}

} /* namespace CompositePattern */
} /* namespace DesignPattern */

