#ifndef TEST_SIZE_H
#define TEST_SIZE_H

#include <iostream>
#include <cstdint>

namespace Test_Size {
    class X {
    };

    // 64位下，vtbl指针一个，占用 8 bytes
    // 　　　　由于有virtual 得以指针size为align，占用 8 bytes
    //       sizeof(Y1) :=16
    class Y1 : public virtual X {
    public:
        Y1(char name) : _name(name) { }

        char _name;
    };

    // 64位下，vtbl指针一个，占用 8 bytes，自己无内部成员，直接使用[vtbl]
    //        soizeof(Y2) := 8
    class Y2 : public virtual X {

    };

    // 64位下，X无变，自己只有一个name，所以占用1个字节
    // sizeof(Y3) := 1
    class Y3 : public X {
    public:
        Y3(char name) : _name(name) { }

        char _name;
    };

    // 64位下，X无变量，自己无变量，但为了区别，C++会编译占用1个字节
    // sizeof(Y4) := 1
    class Y4 : public X {

    };


    void printSize() {
        size_t xs = sizeof(X);
        size_t y1s = sizeof(Y1);
        size_t y2s = sizeof(Y2);
        size_t y3s = sizeof(Y3);
        size_t y4s = sizeof(Y4);

        std::cout
        << "\nsizeof(uintptr_t) :=" << sizeof(uintptr_t)
        << "\nsizeof(intptr_t)  :=" << sizeof(intptr_t)
        << "\nsizeof(X)  :=" << xs
        << "\nsizeof(Y1) :=" << y1s
        << "\nsizeof(Y2) :=" << y2s
        << "\nsizeof(Y3) :=" << y3s
        << "\nsizeof(Y4) :=" << y4s

        << std::endl;
    }

    void printPtr() {
        Y1 y1('A');
        Y2 y2;
        {
            uintptr_t py1 = (uintptr_t) &y1;
            std::cout << "y1.instance      addr := " << py1 << std::endl;
            std::cout << "y1.instance vtbl addr := " << py1 << std::endl;
            std::cout << "y1.instance.name addr := " << (py1 + 8) << std::endl; // 跳过指针８个字节.
            std::cout << "dir y1.instance.name addr := " << (uintptr_t) (&y1._name) << std::endl;
        }
        {
            void *py1 = (void *) &y1;
            std::cout << "y1.instance      addr := " << py1 << std::endl;
            std::cout << "y1.instance vtbl addr := " << py1 << std::endl;
            std::cout << "y1.instance.name addr := " << (py1 + 8) << std::endl;// void* 指针是以１个字节为单位，所以跳过８个字节
            std::cout << "dir y1.instance.name addr := " << (void *) (&y1._name) << std::endl;
        }


    }

    void test() {
        long array[] = {0x0102030405060708, 0x0102030405060708};
        std::cout << " array[0] " << &array[0] << ", array[1] " << &array[1] << std::endl;
        {
            long *ptr0 = &array[0];
            long *ptr1 = ptr0 + 1; // 这儿指针是指向具体类型, 跳到下一个sizeof(long)指针大小
            std::cout << " long* ptr0 " << ptr0 << ", ptr1 " << ptr1 << std::endl;
        }
        {
            void *ptr0 = (void *) &array[0];
            void *ptr1 = ptr0 + 1; // 这儿指针是指向void类型,就是跳  size of a void or of a function as 1，是直接内存加１.
            std::cout << " void* ptr0 " << ptr0 << ", ptr1 " << ptr1 << std::endl;
        }
        printSize();
        printPtr();
    }
};

#endif // TEST_SIZE_H
