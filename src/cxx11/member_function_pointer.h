#ifndef MEMBER_FUNC_H
#define MEMBER_FUNC_H

#include <functional>
#include <iostream>

namespace Test_MFP {

class Base {
public:
    virtual void say() { std::cout << "say default ." << std::endl; }

    void sayx() { std::cout << "Base::sayx." << std::endl; }
};

class BB : public Base {
public:
    virtual void say() override
    { // override c++11 特性
        std::cout << "BB default ." << std::endl;
    }

    void sayx()
    {
        Base::sayx();
        std::cout << "BB::sayx ." << std::endl;
    }

    void bindTest()
    {
        std::cout << "bb::bindTest" << std::endl;
    }

    void bindTest2(int b, int c )
    {
        std::cout << "bb::bindTest, b:" << b << ", c:" << c << std::endl;
    }
};

typedef void (*Func)(void); // 指向函数的指针
typedef void (BB::*CFunc)(void); //指向类成员函数的指针

void test()
{
    BB b;
    //-----------------------------------------------------------------------
    // 成员函数指针 ==>　virtual 成员函数
    // 指向类成员的函数指针，调用virtual函数，最终都是调用的 BB->say();
    CFunc mf = &BB::say;
    (b.*mf)();
    mf = &Base::say;
    (&b->*mf)();
    //-----------------------------------------------------------------------
    // 成员函数指针 ==>　非virtual 成员函数
    mf = &BB::sayx; // 就是调用的 BB::sayx
    (b.*mf)();
    mf = &Base::sayx; // 就是调用的 Base::sayx
    (&b->*mf)();
    //-----------------------------------------------------------------------
    // c++11x 形式
    typedef std::function<void()> fp;
    fp v = std::bind(&BB::bindTest, &b);
    v();

    typedef std::function<void(int)> fp2; 
    fp2 v2 = std::bind(&BB::bindTest2, &b, std::placeholders::_1, 20); // 参数1不变，参数2固定为20.
    v2(10); // b = 10 , c = 20 
    //------------------------------------------------------------------------
}
};

#endif /* MEMBER_FUNC_H */