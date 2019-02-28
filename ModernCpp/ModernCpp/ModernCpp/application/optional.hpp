#ifndef _OPTIONAL_HPP_
#define _OPTIONAL_HPP_

#include <type_traits>
#include <iostream>
#include <string>
#include <map>

namespace application {

template<typename T>
class Optional {
	using data_t = typename std::aligned_storage<sizeof(T), std::alignment_of<T>::value>::type;
public:
	Optional() {
	}

	Optional(const T& v) {
		Create(v);
	}

	Optional(T&& v) {
		Create(std::move(v));
	}

	~Optional() {
		Destroy();
	}

	Optional(const Optional& other) {
		if (other.IsInit())
			Assign(other);
	}

	Optional(Optional&& other) {
		if (other.IsInit()) {
			Assign(std::move(other));
			other.Destroy();
		}
	}

	Optional& operator=(Optional &&other) {
		Assign(std::move(other));
		return *this;
	}

	Optional& operator=(const Optional &other) {
		Assign(other);
		return *this;
	}

	template<class ... Args>
	void emplace(Args&&... args) {
		Destroy();
		Create(std::forward<Args>(args)...);
	}

	bool IsInit() const {
		return m_hasInit;
	}

	explicit operator bool() const {
		return IsInit();
	}

	T& operator*() {
		if (IsInit()) {
			return *((T*) (&m_data));
		}

		throw std::logic_error("is not init");
	}

	T const& operator*() const {
		if (IsInit()) {
			return *((T*) (&m_data));
		}

		throw std::logic_error("is not init");
	}

	bool operator ==(const Optional<T>& rhs) const {
		return (!bool(*this)) != (!rhs) ?
				false : (!bool(*this) ? true : (*(*this)) == (*rhs));
	}

	bool operator <(const Optional<T>& rhs) const {
		return !rhs ? false : (!bool(*this) ? true : (*(*this) < (*rhs)));
	}

	bool operator !=(const Optional<T>& rhs) {
		return !(*this == (rhs));
	}

private:
	template<class ... Args>
	void Create(Args&&... args) {
		new (&m_data) T(std::forward<Args> (args)...);
		m_hasInit = true;
	}

	void Destroy() {
		if (m_hasInit) {
			m_hasInit = false;
			((T*) (&m_data))->~T();
		}
	}

	void Assign(const Optional& other) {
		if (other.IsInit()) {
			Copy(other.m_data);
			m_hasInit = true;
		} else {
			Destroy();
		}
	}

	void Assign(Optional&& other) {
		if (other.IsInit()) {
			Move(std::move(other.m_data));
			m_hasInit = true;
			other.Destroy();
		} else {
			Destroy();
		}
	}

	void Move(data_t&& val) {
		Destroy();
		new (&m_data) T(std::move(*((T*)

		(&val))));
	}

	void Copy(const data_t& val) {
		Destroy();
		new (&m_data) T(*((T*) (&val)));
	}

private:
	bool m_hasInit = false;
	data_t m_data;
};

class MyClass {
public:
	MyClass(int a, int b) :
			x_(a), y_(b) {
	}

	void print() {
		std::cout << "x_ = " << x_ << std::endl;
		std::cout << "y_ = " << y_ << std::endl;
	}
private:
	int x_;
	int y_;
};

void test_optional() {
	Optional<std::string> a("ok");
	Optional<std::string> b("ok");
	Optional<std::string> c("aa");
	c = a;

	if (c < a)
		std::cout << '<' << std::endl;

	if (a == b)
		std::cout << '=' << std::endl;

	std::map<Optional<std::string>, int> m;
	m.insert(std::make_pair(a, 1));
	m.insert(std::make_pair(c, 2));

	auto it = m.find(a);
	std::cout << it->second << std::endl;

	Optional<MyClass> d;
	d.emplace(10, 20);
	(*d).print();
}

} /* namespace application */

#endif /* _OPTIONAL_HPP_ */
