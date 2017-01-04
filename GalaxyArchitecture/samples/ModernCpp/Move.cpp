#include <iostream>
#include <memory>
#include <algorithm>
#include <cassert>

template<typename T, typename S> class Memory {
	S _size;
	T *_memory;
public:
	explicit Memory(S size) :
			_size(size), _memory(new T[_size] { }) {
	}

	~Memory() {
		delete[] _memory, _memory = nullptr;
	}

	Memory(const Memory & copy) :
			_size(copy._size), _memory(new T[copy._size] { }) {
		std::copy_n(copy._memory, copy._size, _memory);
	}

	Memory & operator=(const Memory & copy) {
		if (this != &copy) {
			if (_size != copy._size) {
				_size = copy._size;
				delete _memory, _memory = _size > 0 ? new T[_size] { } : nullptr;
			}

			std::copy_n(copy._memory, copy._size, _memory);
		}
		return *this;
	}

	Memory(Memory && temp) :
			_size(temp._size), _memory(temp._memory) {
		temp._memory = nullptr;
		temp._size = 0;
	}

	Memory& operator=(Memory && temp) {
		assert(this != &temp); // assert if this is not a temporary

		_size = temp._size;
		_memory = temp._memory;

		temp._memory = nullptr;
		temp._size = 0;

		return *this;
	}
};

template<typename T> class Buffer {
	std::string _name;
	size_t _size;
	std::unique_ptr<T[]> _buffer;
	Memory<T, size_t> *_mem;

public:
	// constructor
	Buffer(const std::string& name, size_t size) :
			_name(name), _size(size), _buffer(new T[size] { }), _mem(new Memory<T, size_t> { _size }) {
		std::cout << "constructor" << std::endl;
	}

	// destructor
	~Buffer() {
		std::cout << "destructor" << std::endl;
		delete _mem, _mem = nullptr;
	}

	// copy constructor
	Buffer(const Buffer& copy) :
			_name(copy._name), _size(copy._size), _buffer(new T[copy._size] { }), _mem(
					new Memory<T, size_t> { copy._size }) {
		std::cout << "copy constructor" << std::endl;

		T* source = copy._buffer.get();
		T* dest = _buffer.get();
		std::copy_n(source, copy._size, dest);

		//copy assignment operator
		*_mem = *copy._mem;
	}

	// copy assignment operator
	Buffer& operator=(const Buffer& copy) {
		std::cout << "copy assignment operator" << std::endl;
		if (this != &copy) {
			_name = copy._name;

			if (_size != copy._size) {
				_buffer = nullptr;
				_size = copy._size;
				_buffer = _size > 0 ? (std::unique_ptr<T[]>) new T[copy._size] : nullptr;
				_mem = _size > 0 ? new Memory<T, size_t> { copy._size } : nullptr;
			}

			T* source = copy._buffer.get();
			T* dest = _buffer.get();
			std::copy_n(source, copy._size, dest);

			//copy assignment operator
			*_mem = *copy._mem;
		}

		return *this;
	}

	// move constructor
	Buffer(Buffer&& temp) :
			_name(std::move(temp._name)), _size(temp._size), _buffer(std::move(temp._buffer)), _mem(
					std::move(temp._mem)) {
		std::cout << "move constructor" << std::endl;

		temp._mem = nullptr;
		temp._buffer = nullptr;
		temp._size = 0;
	}

	// move assignment operator
	Buffer& operator=(Buffer&& temp) {
		std::cout << "move assignment operator" << std::endl;
		assert(this != &temp); // assert if this is not a temporary

		_buffer = nullptr;
		_size = temp._size;
		_buffer = std::move(temp._buffer);
		_mem = std::move(temp._mem);
		_name = std::move(temp._name);

		temp._mem = nullptr;
		temp._buffer = nullptr;
		temp._size = 0;

		return *this;
	}

	void print() {
		std::cout << this->_name << " " << this->_size << std::endl;
	}
};

template<typename T>
Buffer<T> getBuffer(const std::string& name) {
	Buffer<T> b(name, 128); //constructor
	return b;
}

#if 0
int main() {

	Buffer<int> instance("instance", 80);	//constructor
	Buffer<int> object("object", 60);		//constructor
	Buffer<int> newInstance = instance;		//copy constructor
	object = instance;						//copy assignment operator

	std::cout << "-----------------------" << std::endl;

	Buffer<int> newObject = getBuffer<int>("newObject");	//move constructor
//	newObject.print();
	Buffer<int> newObject2 = std::move(newObject);			//move constructor
	newObject2 = getBuffer<int>("newObject2");		//move assignment operator

//	int i = 42;
//	int &r = i;				// ok: r refers to i
//	//	int &&rr = i;     	// error: cannot bind an rvalue reference to an lvalue
//	//	int &r2 = i * 42;  	// error: i * 42 is an rvalue
//	const int &r3 = i * 42; // ok: we can bind a reference to  const  to an rvalue
//	int &&rr2 = i * 42;   	// ok: bind rr2 to the result of the multiplication
//	//	int &&rr3 = rr2;   	// error: the expression rr2 is an lvalue!
//	int &&rr3 = std::move(rr2);   // ok
	return 0;
}
#endif
