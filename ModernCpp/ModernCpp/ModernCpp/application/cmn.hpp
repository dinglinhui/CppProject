#ifndef _CMN_HPP_
#define _CMN_HPP_

struct SomeType {
    SomeType() = default;
//  SomeType(OtherType value);
};

struct NonCopyable {
    NonCopyable & operator=(const NonCopyable&) = delete;
    NonCopyable(const NonCopyable&) = delete;
    NonCopyable() = default;
};

struct NonNewable {
    void *operator new(std::size_t) = delete;
};

struct NoDouble {
    void f(int i);
    void f(double) = delete;
};

struct OnlyInt {
    void f(int i);
    template<class T> void f(T) = delete;
};

#endif /* _CMN_HPP_ */
