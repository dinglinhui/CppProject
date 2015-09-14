/*
 * Caretaker.h
 *
 *  Created on: Nov 12, 2014
 *      Author: dlh
 */

#pragma once

#include "Memento.h"

namespace DesignPattern
{
namespace MementoPattern
{

class Memento;
class Caretaker
{
public:
    Caretaker();
    ~Caretaker();
    void SetMemento(Memento*);
    Memento* GetMemento();
private:
    Memento* _memento = nullptr;
};

} /* namespace MementoPattern */
} /* namespace DesignPattern */
