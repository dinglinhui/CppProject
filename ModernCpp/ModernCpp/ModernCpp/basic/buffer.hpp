#ifndef _BUFFER_H_
#define _BUFFER_H_
#include <utility>
#include <memory>
#include <iostream>

namespace application {

template<typename T>
class Buffer {
    std::string _name;
    size_t _size;
    std::unique_ptr<T[]> _buffer;

public:
    Buffer(const std::string& name = "", size_t size = 16) :
            _name(name), _size(size), _buffer(size ? new T[size] : nullptr) {
    }

    Buffer(const Buffer& copy) :
            _name(copy._name), _size(copy._size), _buffer(copy._size ? new T[copy._size] : nullptr) {
        std::cout << "copy constructor" << std::endl;
        T* source = copy._buffer.get();
        T* dest = _buffer.get();
        std::copy(source, source + copy._size, dest);
    }

    // copy assignment operator
    Buffer& operator=(Buffer copy) {
        std::cout << "copy assignment operator" << std::endl;
        swap(*this, copy);
        return *this;
    }

    // move constructor
    Buffer(Buffer&& temp) :
            Buffer() {
        std::cout << "move constructor" << std::endl;
        swap(*this, temp);
    }

    friend void swap(Buffer& first, Buffer& second) noexcept {
        std::swap(first._name, second._name);
        std::swap(first._size, second._size);
        std::swap(first._buffer, second._buffer);
    }
};

template<typename T>
Buffer<T> getBuffer(const std::string& name) {
    Buffer<T> b(name, 128);
    return b;
}

void test_buffer() {
    Buffer<int> b1;
    Buffer<int> b2("buf2", 64);
    Buffer<int> b3 = b2;
    Buffer<int> b4 = getBuffer<int>("buf4");
    b1 = getBuffer<int>("buf5");
}

} /* namespace application */
#endif
