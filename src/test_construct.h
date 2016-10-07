#ifndef CXX_DEMO_TEST_CONSTRUCT_H
#define CXX_DEMO_TEST_CONSTRUCT_H

#include <new>

namespace Test_Construct {
    class Abstract_Base {
    public:
        Abstract_Base() {
            std::cout << " Abstract_Base::Abstract_Base " << std::endl;
        }

        virtual ~Abstract_Base() {
            std::cout << " Abstract_Base::~Abstract_Base " << std::endl;
        }

        void *operator new(size_t size) {
            std::cout << "operator new size " << size << std::endl;
            return ::operator new(size);
        }

        void operator delete(void *pointee) {
            std::cout << "operator delete" << std::endl;
            ::operator delete(pointee);
        }

    public:
        virtual int interface_aaa() = 0;
    };

    class Point : public Abstract_Base {
    public:
        // TODO: 如果不加[Abstract_Base::Abstract_Base()] 编译器会自动加上 调用父类无参数构造器(base()).
        Point(int x) : Abstract_Base::Abstract_Base() , _x(x){
            std::cout << " Point::Point " << std::endl;
        }

        // TODO:每一个类中最多只能有一个析构函数，因此调用的时候并不会出现二义性，因此析构函数不需要显式的调用
        ~Point() {
            std::cout << " Point::~Point " << std::endl;
        }

    public:
        // override c++11 新增加特性
        virtual int interface_aaa() override {
            return this->_x;
        }

    private:
        int _x;
    };

    void test() {
        {
            Abstract_Base *base = new Point(1);
            int v = base->interface_aaa();
            (void) v;
            // TODO: 解构函数如不是virtual，这儿就是跳过子类，直接调用父类的解构函数，从而没有调用［子类］解构函数，引起内存泻漏.
            delete base;
        }
        {
            void *m = malloc(sizeof(Point));

            // 在分配好的内存上初始化对象，并返回指向该对象的指针
            Point *p = ::new(m) Point(100);      //这种就不会再调用Point中申明的 operator new();
            int v = p->interface_aaa();
            (void) v;
            // 调用这个对象的解构器，放出对象所使用的空间
            p->~Point();

            free(m);
        }
    }
};

#endif //CXX_DEMO_TEST_CONSTRUCT_H
