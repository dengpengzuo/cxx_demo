#include "member_function_pointer.h"
#include "template_function_object.h"
#include "test_size.h"
#include "test_member_addr.h"

int main(int argc, char **argv) {
    Test_MFP::test();
    Test_TFO::test();
    Test_Size::test();
    Test_Member_Addr::test();
    return 0;
}
