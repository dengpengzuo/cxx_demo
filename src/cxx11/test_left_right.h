#pragma once

#include <string>
#include <vector>
#include <cstring>


namespace test_left_right {

    class MyString {
    private:
        char *_data;
        size_t _len;

        void _init_data(const char *s) {
            _data = new char[_len + 1];
            std::memcpy(_data, s, _len);
            _data[_len] = '\0';
        }

    public:
        MyString() {
            _data = NULL;
            _len = 0;
        }

        MyString(const char *p) {
            _len = strlen(p);
            _init_data(p);
        }

        MyString(const MyString &str) {
            _len = str._len;
            _init_data(str._data);
            std::cout << "LEFT Copy Constructor is called! source: " << str._data << std::endl;
        }


        MyString &operator=(const MyString &str) {
            if (this != &str) {
                std::cout << "Copy Assignment is called! source: " << str._data << std::endl;

                _len = str._len;
                _init_data(str._data);
            }
            return *this;
        }

#define _HAVERIGHT_    2
#if (_HAVERIGHT_ > 1)

        MyString(MyString &&str) {
            std::cout << "RIGHT Copy Constructor is called! source: " << str._data << std::endl;

            _len = str._len;
            _init_data(str._data); // std:;move()

            str._len = 0;
            str._data = nullptr;
        }

        MyString &operator=(MyString &&str) {

            if (this != &str) {
                std::cout << "RIGHT Copy Assignment is called! source: " << str._data << std::endl;

                _len = str._len;
                _init_data(str._data);  // std:;move()

                str._len = 0;
                str._data = nullptr;
            }

            return *this;
        }

#endif

        virtual ~MyString() {
            if (_data) free(_data);
        }
    };

    void test() {
        MyString a;
        a = MyString("Hello");
        std::vector<MyString> vec;
        vec.push_back(MyString("World"));
    }
};