#pragma once

#include <vector>
#include "Component.h"

namespace DesignPattern
{
namespace CompositePattern
{

class Component;
class Composite:public Component
{
public:
    Composite();
    ~Composite();

    void Operation();
    void Add(Component*);
    void Remove(Component*);
    Component* GetChild(int index);
private:
    std::vector<Component*> m_ComVec;
};

} /* namespace CompositePattern */
} /* namespace DesignPattern */

