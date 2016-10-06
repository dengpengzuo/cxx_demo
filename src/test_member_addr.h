#ifndef TEST_MEMBER_ADDR_H
#define TEST_MEMBER_ADDR_H

#include <iostream>
#include <cstdint>

namespace Test_Member_Addr {

    class Point3D {
    public:
        Point3D(int x, int y, int z)
                : _x(x), _y(y), _z(z) {
        }

        virtual ~Point3D() { }

    public:
        int _x;
        int _y;
        int _z;
    };

    void test() {
        Point3D *addr = new Point3D(1, 2, 3);
        {
            Point3D *ptr = addr;
            // 无virtual 函数情况下: ptr 和 x 是同一个内存地址.
            // 有virtual 函数情况下：ptr 指向是 vtbl ,vtbl +　8 才是 x　的地址.
            std::cout << "Point3d instance " << addr << " mem address:" << ptr << std::endl;
            std::cout << "Point3d instance " << addr << "._x mem address:" << (void *) (&ptr->_x) << std::endl;
            std::cout << "Point3d instance " << addr << "._z mem address:" << (void *) (&ptr->_z) << std::endl;
        }
        {
            typedef void (Point3D::* CFunc)(void); // 成员函数指针
            typedef int Point3D::* POINT_MP;        // 成员变量指针
            // 指向对象成员变量的指针.
            POINT_MP px = &Point3D::_x;
            POINT_MP py = &Point3D::_y;
            POINT_MP pz = &Point3D::_z;
            // (*addr).*px , addr->*px 就是访问 addr　的变量值.
            fprintf(stdout, "Point3d::x offset := %p val := %d \n", px, addr->*px);
            fprintf(stdout, "Point3d::y offset := %p val := %d \n", py, addr->*py);
            fprintf(stdout, "Point3d::z offset := %p val := %d \n", pz, addr->*pz);
        }
        delete addr;
    }
};

#endif // TEST_MEMBER_ADDR_H
