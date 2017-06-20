/*
 * 学习boost协程例子
 * 协程是有点复杂
 */
#include <cstdio>
#include <cstdlib>
#include <iostream>

#include <boost/context/all.hpp>

typedef std::function<void(void*)> uthread_function;

template <typename Alloc>
class boost_uthread {
public:
    ~boost_uthread()
    {
        this->alloc_.deallocate(this->stack_);
    }

    boost_uthread(Alloc const& alloc = Alloc())
        : alloc_(alloc)
    {
        this->stack_ = this->alloc_.allocate();
    }

    void run(uthread_function callback, void* args)
    {
        this->callback_ = (callback);
        this->args_ = (args);
        // 得到协程ID，同uthreadFunction中代码，取回协程执行权.
        auto fctx = boost::context::detail::make_fcontext(this->stack_.sp, this->stack_.size, &uthreadFunction);
        boost::context::detail::transfer_t t = boost::context::detail::jump_fcontext(fctx, reinterpret_cast<void*>(this));
        this->fctx_ = t.fctx;
    }

    void resume()
    {
        // 将执行权给协程.
        boost::context::detail::transfer_t t = boost::context::detail::jump_fcontext(this->fctx_, nullptr);
        // fprintf(stdout, "resume jump fcontext:%p return: %p\n", this->fctx_, t.fctx);
        this->fctx_ = t.fctx;
    }

    void yield()
    {
        // 协程让出执行权
        boost::context::detail::transfer_t t = boost::context::detail::jump_fcontext(this->from_fctx_, nullptr);
        // fprintf(stdout, "yield jump fcontext:%p return: %p\n", this->from_fctx_, t.fctx);
        this->from_fctx_ = t.fctx;
    }

private:
    static void uthreadFunction(boost::context::detail::transfer_t t)
    {
        boost_uthread* ptr = static_cast<boost_uthread*>(t.data);
        // return run(),放出协程执行权.
        boost::context::detail::transfer_t t_ = boost::context::detail::jump_fcontext(t.fctx, nullptr);
        ptr->from_fctx_ = t_.fctx;
        ptr->callback_(ptr->args_);
    }

    boost::context::detail::fcontext_t fctx_;
    boost::context::detail::fcontext_t from_fctx_;

    uthread_function callback_;
    void* args_;

    Alloc alloc_;
    boost::context::stack_context stack_;
};

typedef boost_uthread<boost::context::protected_fixedsize_stack> pfx_ucthread;

struct input_v {
    int target;
    bool quit_;
};

struct input_args {
    input_v* in_;
    pfx_ucthread* c_;
};

void input_task(void* p)
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
        pia->c_->yield();
    }

    printf("ERROR: should not reach the end of input function\n");
}

struct sum_args {
    input_v* in_;
    pfx_ucthread* c_;
    int count_;
    int sum_;
};

void sum_task(void* p)
{
    sum_args* psa = static_cast<sum_args*>(p);
    while (true) {
        ++ (psa->count_);
        psa->sum_ += psa->in_->target;
        psa->c_->yield();
    }

    printf("ERROR: should not reach the end of sum function\n");
}

int main()
{
    pfx_ucthread c1, c2;
    input_v in = {.target = 0, .quit_ = false };
    input_args ia = {.in_ = &in, .c_ = &c1 };
    sum_args sum = {.in_ = &in, .c_ = &c2, .count_ = 0, .sum_ = 0 };

    c1.run(&input_task, (void*)&ia);
    c2.run(&sum_task, (void*)&sum);

    while (!in.quit_) {
        c1.resume(); // 初始是协程让出控制权，现在是让协程取得使用权.
        c2.resume(); // 初始是协程让出控制权，现在是让协程取得使用权.
        fprintf(stdout, "input val:%d count:%d sum:%d \n", in.target, sum.count_, sum.sum_);
    }

    printf("main: done\n");
    return 0;
}
