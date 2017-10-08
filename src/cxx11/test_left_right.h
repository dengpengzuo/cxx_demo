#pragma once

#include <cstring>
#include <string>
#include <vector>
#include <utility>

namespace test_left_right {

    class MyString {
    private:
        std::string _data;

    public:
        MyString() : _data() {}

        MyString(const char *p) : _data(p) {}

        MyString(const MyString &str) {
            std::cout << "LEFT Copy Constructor is called! source: " << str._data
                      << std::endl;
            _data = str._data;
        }

        MyString &operator=(const MyString &str) {
            if (this != &str) {
                std::cout << "Copy Assignment is called! source: " << str._data
                          << std::endl;
                _data = str._data;
            }
            return *this;
        }

        MyString(MyString &&str) {
            std::cout << "RIGHT Copy Constructor is called! source: " << str._data
                      << std::endl;
            _data = std::move(str._data);
        }

        MyString &operator=(MyString &&str) {
            if (this != &str) {
                std::cout << "RIGHT Copy Assignment is called! source: " << str._data
                          << std::endl;
                _data = std::move(str._data);
            }
            return *this;
        }

        virtual ~MyString() { std::cout << "Free Data:" << _data << std::endl; }
    };

    MyString foo() {
        MyString v("aaabbb");
        return v;
    }

    void test() {
        MyString a;
        a = foo(); // foo 的返回值，产生成了一下right copy.

        std::vector<MyString> vec;
        vec.push_back(MyString("World")); // 产生了一个MyString，在进入vector中，又产生了一个right copy.
        vec.clear();

        auto fib = [a = 0, b = 1]() mutable {
            a = std::exchange(b, a + b); // a := b, b := a + b 
            return a;
        };
        for (int i = 0; i < 10; ++i)
            std::cout << fib() << std::endl;
    }
};