#include <iostream>

#if 0
class Nature
{
public:
	Nature()
	{
	}

	virtual ~Nature()
	{
	}

	virtual void evolve(int) = 0;
};

class Animal : public Nature
{
public:
	Animal()
	{
	}
	virtual ~Animal()
	{

	}

	virtual void evolve(float) override
	{
		std::cout << "Animal::evolve" << std::endl;
	}
};

class Plant : public Nature
{
public:
	Plant()
	{
	}

	~Plant()
	{
	}

	virtual void evolve(int) override final
	{
		std::cout << "Plant::evolve" << std::endl;
	}

};

class Mammal : public Animal
{
public:
	Mammal()
	{
	}

	virtual ~Mammal()
	{
	}

	virtual void evolve(int)
	{
		std::cout << "Mammal::evolve" << std::endl;
	}
};

class Gymnosperm : public Plant
{
public:
	Gymnosperm()
	{
	}

	virtual ~Gymnosperm()
	{
	}

	virtual void evolve(int)
	{
		std::cout << "Gymnosperm::evolve" << std::endl;
	}
};

int main()
{

	return 0;
}

#endif
