#pragma once

namespace DesignPattern {
namespace SingletonPattern {

class Singleton {
private:
	static Singleton* m_spInstance;
	Singleton();
public:
	static Singleton* getInstance();
	static void destroyInstance();
};

} /* namespace SingletonPattern */
} /* namespace DesignPattern */
