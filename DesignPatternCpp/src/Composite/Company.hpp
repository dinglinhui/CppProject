#include <string>
#include <list>
#include <iostream>

namespace DesignPattern
{
namespace CompositePattern
{

class Company
{
public:
	Company(std::string name)
	{
		m_name = name;
	}
	virtual ~Company()
	{
	}
	virtual void Add(Company *pCom)
	{
	}
	virtual void Show(int depth)
	{
	}
protected:
	std::string m_name;
};

class ConcreteCompany: public Company
{
public:
	ConcreteCompany(std::string name) :
			Company(name)
	{
	}

	virtual ~ConcreteCompany()
	{
	}

	void Add(Company *pCom)
	{
		m_listCompany.push_back(pCom);
	}

	void Show(int depth)
	{
		for (int i = 0; i < depth; i++)
			cout << "-";
		cout << m_name << endl;
		std::list<Company *>::iterator iter = m_listCompany.begin();
		for (; iter != m_listCompany.end(); iter++)
			(*iter)->Show(depth + 2);
	}
private:
	std::list<Company *> m_listCompany;
};

class FinanceDepartment: public Company
{
public:
	FinanceDepartment(std::string name) :
			Company(name)
	{
	}
	virtual ~FinanceDepartment()
	{
	}
	virtual void Show(int depth)
	{
		for (int i = 0; i < depth; i++)
			cout << "-";
		cout << m_name << endl;
	}
};

class HRDepartment: public Company
{
public:
	HRDepartment(std::string name) :
			Company(name)
	{
	}
	virtual ~HRDepartment()
	{
	}
	virtual void Show(int depth)
	{
		for (int i = 0; i < depth; i++)
			cout << "-";
		cout << m_name << endl;
	}
};

} /* namespace CompositePattern */
} /* namespace DesignPattern */
