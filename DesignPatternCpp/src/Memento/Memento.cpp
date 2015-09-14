#include "Memento.h"

namespace DesignPattern
{
namespace MementoPattern
{

Memento::Memento(const std::string& state)
{
	this->_state = state;
}

Memento::~Memento()
{
}

std::string Memento::GetState()
{
	return this->_state;
}

void Memento::SetState(const std::string& state)
{
	this->_state = state;
}

} /* namespace MementoPattern */
} /* namespace DesignPattern */
