#ifndef TEST_MEMBER_ADDR_H
#define TEST_MEMBER_ADDR_H

#include <cstdint>
#include <iostream>
#include <stdio.h>

namespace Test_Member_Addr {

class Point3D {
public:
    Point3D(int x, int y, int z)
        : _x(x)
        , _y(y)
        , _z(z)
    {
    }

    virtual ~Point3D() {}

public:
    int _x;
    int _y;
    int _z;
};

class WXYZ : public Point3D {
public:
    WXYZ(int w, int x, int y, int z)
        : Point3D::Point3D(x, y, z)
        , _w(w)
    {
    }

    ~WXYZ() {}

public:
    int _w;
};

void testPoint3d()
{
    Point3D* addr = new Point3D(1, 2, 3);
    {
        Point3D* ptr = addr;
        // 无virtual 函数情况下: ptr 和 x 是同一个内存地址.
        // 有virtual 函数情况下：ptr 指向是 vtbl ,vtbl +　8 才是 x　的地址.
        std::cout << "Point3d instance " << addr
                  << "            mem address:" << ptr << std::endl;
        std::cout << "Point3d instance " << addr
                  << " Point3D._x mem address:" << (void*)(&ptr->_x) << std::endl;
        std::cout << "Point3d instance " << addr
                  << " Point3D._z mem address:" << (void*)(&ptr->_z) << std::endl;
    }
    {
        typedef void (Point3D::*CFunc)(void); // 成员函数指针
        typedef int Point3D::*POINT_MP; // 成员变量指针
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

void testWXYZ()
{
    WXYZ* addr = new WXYZ(10, 1, 2, 3);
    {
        WXYZ* ptr = addr;
        // 无virtual 函数情况下: ptr 和 x 是同一个内存地址.
        // 有virtual 函数情况下：ptr 指向是 vtbl ,vtbl +　8 才是 x　的地址.
        std::cout << "WXYZ instance " << addr
                  << "                  mem address:" << ptr << std::endl;
        std::cout << "WXYZ instance " << addr
                  << " WXYZ::Point3D._x mem address:" << (void*)(&ptr->_x)
                  << std::endl;
        std::cout << "WXYZ instance " << addr
                  << " WXYZ::Point3D._y mem address:" << (void*)(&ptr->_y)
                  << std::endl;
        std::cout << "WXYZ instance " << addr
                  << " WXYZ::Point3D._z mem address:" << (void*)(&ptr->_z)
                  << std::endl;
        std::cout << "WXYZ instance " << addr
                  << " WXYZ._w          mem address:" << (void*)(&ptr->_w)
                  << std::endl;
    }
    {
        typedef void (WXYZ::*CFunc)(void); // 成员函数指针
        typedef int WXYZ::*POINT_MP; // 成员变量指针
        // 指向对象成员变量的指针.
        POINT_MP px = &WXYZ::_x;
        POINT_MP py = &WXYZ::_y;
        POINT_MP pz = &WXYZ::_z;
        POINT_MP pw = &WXYZ::_w;
        // (*addr).*px , addr->*px 就是访问 addr　的变量值.
        fprintf(stdout, "WXYZ::Point3d::x offset := %p val := %d \n", px,
            addr->*px);
        fprintf(stdout, "WXYZ::Point3d::y offset := %p val := %d \n", py,
            addr->*py);
        fprintf(stdout, "WXYZ::Point3d::z offset := %p val := %d \n", pz,
            addr->*pz);
        fprintf(stdout, "WXYZ::w          offset := %p val := %d \n", pw,
            addr->*pw);
    }
    delete addr;
}

void test()
{
    testPoint3d();
    testWXYZ();
}
};

#endif // TEST_MEMBER_ADDR_H
