/*
 * Singleton.h
 *
 *  Created on: Apr 8, 2015
 *      Author: dinglinhui
 */

#ifndef SINGLETON_H_
#define SINGLETON_H_

namespace cplusplus {

template<typename T>
class Singleton {
public:
	template<typename ... Args>
	static T* Instance(Args&&... args) {
		if (m_pInstance == nullptr)
			m_pInstance = new T(std::forward<Args>(args)...);
		return m_pInstance;
	}

	static T* GetInstance() {
		if (m_pInstance == nullptr)
			throw std::logic_error(
					"the instance is not init, please initialize the instance first");
		return m_pInstance;
	}

	static void DestroyInstance() {
		delete m_pInstance;
		m_pInstance = nullptr;
	}

private:
	Singleton(void);
	virtual ~Singleton(void);
	Singleton(const Singleton&);
	Singleton& operator =(const Singleton&);

private:
	static T* m_pInstance;
};

template<class T> T* Singleton<T>::m_pInstance = nullptr;

} //namespace cplusplus
#endif /* SINGLETON_H_ */
