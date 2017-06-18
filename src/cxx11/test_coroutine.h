#ifndef TEST_POINT_H
#define TEST_POINT_H
#include <stdlib.h>
#include <string.h>

namespace Test_Coroutine {
int function(void)
{
    static int i, state = 0;
    switch (state) {
    case 0: /* start of function */
        fprintf(stdout, "case 0 \n");
        for (i = 0; i < 10; i++) {
            state = 1; /* so we will come back to "case 1" */
            return i;
        case 1:
            fprintf(stdout, "case 1 resume for for(i) loop \n"); /* resume control straight after the return */
        }
    }
}

int test()
{
    // 原始的 coroutine 写法
    int v = function();
    v = function();
    v = function();
    v = function();
    v = function();
}
};

#endif
