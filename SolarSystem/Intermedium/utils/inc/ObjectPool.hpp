#ifndef OBJECTPOOL_HPP_
#define OBJECTPOOL_HPP_

#include <string>
#include <functional>
#include <tuple>
#include <map>
#include "OSDefs.h"
#include "Any.hpp"

namespace utils {
class ObjectPool {
	template<typename T, typename ... Args>
	using Constructor = std::function<std::shared_ptr<T>(Args...)>;

public:
	ObjectPool() {
	}

	~ObjectPool() {
	}

	//默认创建多少个对象
	template<typename T, typename ... Args>
	void Create(int num) {
		if (num <= 0 || num > OS_MAX_OBJECT_NUM)
			throw std::logic_error("object num errer");

		auto constructName = typeid(Constructor<T, Args...> ).name();

		Constructor<T, Args...> f = [constructName, this](Args... args) {
			return std::shared_ptr<T>(new T(args...),[constructName, this](T* t)
					{
						m_object_map.emplace(constructName, std::shared_ptr<T>(t));
					});
		};

		m_map.emplace(typeid(T).name(), f);
		m_counter.emplace(constructName, num);
	}

	template<typename T, typename ... Args>
	std::shared_ptr<T> Get(Args ... args) {
		using ConstructType = Constructor<T, Args...>;

		std::string constructName = typeid(ConstructType).name();
		auto range = m_map.equal_range(typeid(T).name());
		for (auto it = range.first; it != range.second; ++it) {
			if (it->second.Is<ConstructType>()) {
				auto ptr = GetInstance<T>(constructName, args...);

				if (ptr != nullptr)
					return ptr;

				return CreateInstance<T, Args...>(it->second, constructName, args...);
			}
		}

		return nullptr;
	}

private:
	template<typename T, typename ... Args>
	std::shared_ptr<T> CreateInstance(Any& any, std::string& constructName, Args ... args) {
		using ConstructType = Constructor<T, Args...>;
		ConstructType f = any.AnyCast<ConstructType>();

		InitPool(f, constructName, args...);

		return f(args...);
	}

	template<typename T, typename ... Args>
	void InitPool(T& f, std::string& constructName, Args ... args) {
		int num = m_counter[constructName];
		if (num != 0) {
			for (int i = 0; i < num - 1; i++) {
				m_object_map.emplace(constructName, f(args...));
			}
			m_counter[constructName] = 0;
		}
	}

	template<typename T, typename ... Args>
	std::shared_ptr<T> GetInstance(std::string& constructName, Args ... args) {
		auto it = m_object_map.find(constructName);
		if (it == m_object_map.end())
			return nullptr;

		auto ptr = it->second.AnyCast<std::shared_ptr<T>>();
		if (sizeof...(Args)>0)
		*ptr.get() = std::move(T(args...));

		m_object_map.erase(it);
		return ptr;
	}

private:
	std::multimap<std::string, Any> m_map;
	std::multimap<std::string, Any> m_object_map;
	std::map<std::string, int> m_counter;
};
} //namespace utils

namespace test {
using namespace utils;

struct AT {
	AT() {
	}

	AT(int a, int b) :
			m_a(a),
			m_b(b) {
	}

	void Fun() {
		std::cout << m_a << " " << m_b << std::endl;
	}

	int m_a = 0;
	int m_b = 0;
};

struct BT {
	void Fun() {
		std::cout << "from object b " << std::endl;
	}
};

void testObjectPool() {
	std::cout << "testObjectPool:" << std::endl;

	ObjectPool pool;
	pool.Create<AT>(2);
	pool.Create<BT>(2);
	pool.Create<AT, int, int>(2);

	{
		auto p = pool.Get<AT>();
		p->Fun();
	}

	auto pb = pool.Get<BT>();
	pb->Fun();

	auto p = pool.Get<AT>();
	p->Fun();

	int a = 5, b = 6;
	auto p2 = pool.Get<AT>(a, b);
	p2->Fun();

	auto p3 = pool.Get<AT>(3, 4);
	p3->Fun();

	{
		auto p4 = pool.Get<AT>(3, 4);
		p4->Fun();
	}
	auto p4 = pool.Get<AT>(7, 8);
	p4->Fun();

	std::cout << std::endl;
}

} //namespace test
#endif /* OBJECTPOOL_HPP_ */
