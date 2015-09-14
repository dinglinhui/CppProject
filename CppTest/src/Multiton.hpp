/*
 * Multiton.hpp
 *
 *  Created on: Apr 8, 2015
 *      Author: dinglinhui
 */

#ifndef MULTITON_HPP_
#define MULTITON_HPP_

#include <map>
#include <string>
#include <memory>

namespace cplusplus {

template<typename T, typename K = std::string>
class Multiton {
public:
	template<typename ... Args>
	static std::shared_ptr<T> Instance(const K& key, Args&&... args) {
		return GetInstance(key, std::forward<Args>(args)...);
	}

	template<typename ... Args>
	static std::shared_ptr<T> Instance(K&& key, Args&&... args) {
		return GetInstance(key, std::forward<Args>(args)...);
	}
private:
	template<typename Key, typename ... Args>
	static std::shared_ptr<T> GetInstance(Key&& key, Args&&...args) {
		std::shared_ptr<T> instance = nullptr;
		auto it = m_map.find(key);
		if (it == m_map.end()) {
			instance = std::make_shared<T>(std::forward<Args>(args)...);
			m_map.emplace(key, instance);
		} else {
			instance = it->second;
		}

		return instance;
	}

private:
	Multiton(void);
	virtual ~Multiton(void);
	Multiton(const Multiton&);
	Multiton& operator =(const Multiton&);

private:
	static std::map<K, std::shared_ptr<T>> m_map;
};

template<typename T, typename K>
std::map<K, std::shared_ptr<T>> Multiton<T, K>::m_map;

} //namespace cplusplus
#endif /* MULTITON_HPP_ */
