#pragma once

namespace DesignPattern
{
namespace StatePattern
{

class Context;
class State
{
protected:
    State();

public:
    virtual ~State();

    virtual void Handle(Context* pContext)=0;
};

class ConcreteStateA : public State
{
public:
    ConcreteStateA();
    virtual ~ConcreteStateA();

    virtual void Handle(Context* pContext) override;
};

class ConcreteStateB : public State
{
public:
    ConcreteStateB();
    virtual ~ConcreteStateB();

    virtual void Handle(Context* pContext) override;
};

class ConcreteStateC : public State
{
public:
    ConcreteStateC();
    virtual ~ConcreteStateC();

    virtual void Handle(Context* pContext) override;
};

class Context
{
public:
    Context(State* pState);
    virtual ~Context();

    void Request();
    void ChangeState(State* pState);

private:
    State* _state = nullptr;
};

} /* namespace StatePattern */
} /* namespace DesignPattern */
