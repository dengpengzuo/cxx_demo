#ifndef TEST_SIZE_H
#define TEST_SIZE_H

#include <iomanip>
#include <iostream>
#include <string>
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

    // 64位下，X无内部成员变量，自己只有一个name，所以占用1个字节
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

    struct PrintInfo {
        std::string name;
        size_t size;

        PrintInfo(const char *v, size_t s) : name(v), size(s) { }
    };

    void printSize() {
        struct PrintInfo info[] = {
                //{"sizeof(void)", sizeof(void)}, // TODO:g++支持有警告, clang不支持, sizeof(void) return 1
                {"sizeof(int)", sizeof(int)},
                {"sizeof(long)", sizeof(long)},
                {"sizeof(size_t)", sizeof(size_t)},
                {"sizeof(intptr_t)", sizeof(intptr_t)},

                {"sizeof(X)", sizeof(X)},
                {"sizeof(Y1)", sizeof(Y1)},
                {"sizeof(Y2)", sizeof(Y2)},
                {"sizeof(Y3)", sizeof(Y3)},
                {"sizeof(Y4)", sizeof(Y4)}
        };

        for (size_t i = 0; i < sizeof(info) / sizeof(info[0]); ++i) {
            PrintInfo *p = &info[i];
            std::cout
            << std::left << std::setw(16) << p->name
            << " := "
            << std::right << std::setw(2) << p->size
            << std::endl;
        }
    }

    void test() {
        uint64_t array[] = {0x0102030405060708, 0x0102030405060708};
        std::cout << "   array[0] " << &array[0] << ", array[1] " << &array[1] << std::endl;
        {
            uint64_t *ptr0 = &array[0];
            uint64_t *ptr1 = ptr0 + 1; //sizeof(long) as 8
            std::cout << " long* ptr0 " << ptr0 << ", ptr1 " << ptr1 << std::endl;
        }
        /*{
            void *ptr0 = (void *) &array[0];
            void *ptr1 = ptr0 + 1; // sizeof(void or of a function) as 1, TODO: clang++ 会报错, arithmetic on a pointer to void
            void *ptr1 = (void *) &array[1];
            std::cout << " void* ptr0 " << ptr0 << ", ptr1 " << ptr1 << std::endl;
        }*/
        printSize();
    }
};

#endif // TEST_SIZE_H
