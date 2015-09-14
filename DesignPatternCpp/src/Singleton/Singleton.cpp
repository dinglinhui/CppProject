#include "Singleton.h"
#include <iostream>

using namespace std;
namespace DesignPattern {
namespace SingletonPattern {

Singleton* Singleton::m_spInstance = nullptr;

Singleton::Singleton() {
	cout << "Singleton..." << endl;
}

Singleton* Singleton::getInstance() {
	if (nullptr == m_spInstance)
		m_spInstance = new Singleton { };

	return m_spInstance;
}

void Singleton::destroyInstance() {
	if (nullptr != m_spInstance)
		delete m_spInstance, m_spInstance = nullptr;

	cout << "Destroy..." << endl;
}

}
}
