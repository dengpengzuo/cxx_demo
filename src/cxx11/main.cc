#include "member_function_pointer.h"
#include "template_function_object.h"
#include "test_construct.h"
#include "test_member_addr.h"
#include "test_size.h"
#include "test_coroutine.h"
#include "test_left_right.h"

#include "absl/base/config.h"
#include "absl/base/call_once.h"

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
    std::cout << std::string(50, '-') << std::endl;

    test_left_right::test();
    std::cout << std::string(50, '-') << std::endl;

    return 0;
}
