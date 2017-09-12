/*
 * 学习boost协程例子
 * 协程是有点复杂
 */
#include <cstdio>
#include <iostream>
#include "uthread.h"

struct input_v {
    int target;
    bool quit_;
};

struct input_args {
    input_v* in_;
};

void input_task(pfx_ucthread* c, void* p)
{
    input_args* pia = static_cast<input_args*>(p);

    while (true) {
        printf("number: ");
        int r = scanf("%d", &(pia->in_->target));
        if (!r) {
            pia->in_->quit_ = true;
            // return;
        }
        if (pia->in_->target == 0) {
            pia->in_->quit_ = true;
            // return;
        }
        c->yield();
    }

    printf("ERROR: should not reach the end of input function\n");
}

struct sum_args {
    input_v* in_;
    int count_;
    int sum_;
};

void sum_task(pfx_ucthread* c, void* p)
{
    sum_args* psa = static_cast<sum_args*>(p);
    while (true) {
        ++(psa->count_);
        psa->sum_ += psa->in_->target;
        c->yield();
    }

    printf("ERROR: should not reach the end of sum function\n");
}

int main()
{
    pfx_ucthread c1, c2;
    input_v in = {.target = 0, .quit_ = false };
    input_args ia = {.in_ = &in };
    sum_args sum = {.in_ = &in, .count_ = 0, .sum_ = 0 };

    c1.run(&input_task, (void*)&ia);
    c2.run(&sum_task, (void*)&sum);

    while (!in.quit_) {
        c1.resume(); // 初始是协程让出控制权，现在是让协程取得使用权.
        c2.resume();
        fprintf(stdout, "input val:%d count:%d sum:%d \n", in.target, sum.count_, sum.sum_);
    }

    printf("main: done\n");
    return 0;
}
