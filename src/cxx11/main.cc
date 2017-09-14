#include "cxx11/member_function_pointer.h"
#include "cxx11/template_function_object.h"
#include "cxx11/test_construct.h"
#include "cxx11/test_member_addr.h"
#include "cxx11/test_size.h"
#include "cxx11/test_coroutine.h"
#include "jvm/jvm.h"
#include "cxx11/test_left_right.h"


class GenRemSet: public CHeapObj<mtGC> {

};

int main(int argc, char** argv)
{
    Test_MFP::test();
    std::cout << std::string(50, '-') << std::endl;

    Test_TFO::test();
    std::cout << std::string(50, '-') << std::endl;

    Test_Size::test();
    std::cout << std::string(50, '-') << std::endl;

    Test_Member_Addr::test();
    std::cout << std::string(50, '-') << std::endl;

    Test_Construct::test();
    std::cout << std::string(50, '-') << std::endl;

    Test_Coroutine::test();

    GenRemSet *p = new GenRemSet();
    delete p;

    GenRemSet *x = new GenRemSet[10];
    delete[] x;

    test_left_right::test();
    return 0;
}
