#ifndef TEMPLATE_FUNC_H
#define TEMPLATE_FUNC_H

#include <iostream>
#include <cstdint>

namespace Test_TFO {
    typedef void *pvoid;

    template<typename R, typename T>
    inline static R cast_to(T *&p) {
        return (R) (void *) (p);
    }

    // ----------------------------------------------------------
    // 模板函数对象定义
    // 申明有一个 template 类 hasher，可以不用实现.
    template<typename T>
    struct hasher;

    // 标准实现是传引用进来.
    template<typename T>
    struct hasher {
        // 实现操作符()函数
        size_t operator()(T &v) {
            std::cout << "调用的标准hasher(T &)" << std::endl;
            T *pv = &v;
            return cast_to<uintptr_t, T>(pv);
        }
    };

    // 定义有一个 template 类 hasher.的偏特化《指针》实现.
    template<typename T>
    struct hasher<T *> {
        // 实现操作符()函数
        size_t operator()(T *&pv) {
            std::cout << "调用的偏特化hasher(T *&)" << std::endl;
            return cast_to<uintptr_t, T>(pv);
        }
    };

    // 定义有一个 template 类 hasher.的偏特化uint32_t实现.
    template<> // 偏特化一定要有<>
    struct hasher<uint32_t> {
        // 实现操作符()函数
        size_t operator()(uint32_t v) {
            std::cout << "调用的偏特化 uint32_t ()" << std::endl;
            return v;
        }
    };

    // ----------------------------------------------------------
    // @模板函数例子
    template<typename T, typename H = hasher<T> >
    size_t calc_hash(T &v, H _hasher) {
        return _hasher(v);
    };

    // ----------------------------------------------------------
    // @模板对象例子
    template<typename T, typename H>
    struct CalcHash {
        CalcHash() : _hasher(H()) { }

        size_t calc(T &v) {
            return _hasher(v);
        }

    private:
        H _hasher;
    };

    /*
    // ----------------------------------------------------------
    // 模板对象的偏特化
    // 偏特化时不能在template行写上初始值 如 H = hasher<T*> .
    template<typename T, typename H>
    struct CalcHash<T *, H> {
        CalcHash() : _hasher(H()) { }

        size_t calc(T *pv) {
            return _hasher(pv);
        }

    private:
        H _hasher;
    };
    */

    struct Item {

    };

    typedef struct Item *pitem;

    void test() {
        uint32_t v = 10;
        size_t hash = calc_hash<uint32_t>(v, hasher<uint32_t>());
        std::cout << "calc_hash<uint32_t>:=" << hash << std::endl;

        Item vx;
        pitem pv = &vx;

        // 使用 calc_hash 进行计算
        hash = calc_hash<Item, hasher<Item> >(vx, hasher<Item>());             // 传入函数对象
        std::cout << "calc_hash<Item, hasher<Item>>:=" << hash << std::endl;

        hash = calc_hash<pitem, hasher<pitem> >(pv, hasher<pitem>());   // 传入函数对象
        std::cout << "calc_hash<Item*>(Item*&):=" << hash << std::endl;

        // 使用class CalcHash 进行计算
        hash = CalcHash<Item, hasher<Item> >().calc(vx);
        std::cout << "CalcHash<item>.calc:=" << hash << std::endl;

        hash = CalcHash<pitem, hasher<pitem> >().calc(pv);
        std::cout << "CalcHash<item*>.calc:=" << hash << std::endl;
    }
};

#endif /* TEMPLATE_FUNC_H */