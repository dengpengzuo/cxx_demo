#ifndef MEMBER_FUNC_H
#define MEMBER_FUNC_H

#include <iostream>

namespace Test_MFP {

    class Base {
    public:
        virtual void say() {
            std::cout << "say default ." << std::endl;
        }

        void sayx() {
            std::cout << "Base::sayx." << std::endl;
        }
    };

    class BB : public Base {
    public:
        virtual void say() override { // override c++11 特性
            std::cout << "BB default ." << std::endl;
        }

        void sayx() {
            Base::sayx();
            std::cout << "BB::sayx ." << std::endl;
        }
    };

    typedef void (*Func)(void);       // 指向函数的指针
    typedef void (BB::* CFunc)(void); //指向类成员函数的指针

    void test() {
        BB b;

        // 成员函数指针 ==>　virtual 成员函数
        // 指向类成员的函数指针，调用virtual函数，最终都是调用的 BB->say();
        CFunc mf = &BB::say;
        (b.*mf)();
        mf = &Base::say;
        (&b->*mf)();

        // 成员函数指针 ==>　非virtual 成员函数
        mf = &BB::sayx;   // 就是调用的 BB::sayx
        (b.*mf)();
        mf = &Base::sayx; // 就是调用的 Base::sayx
        (&b->*mf)();
    }
};

#endif /* MEMBER_FUNC_H */