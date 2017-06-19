/*
 * 学习boost协程例子
 * 协程是有点复杂
 */
#include <cstdio>
#include <cstdlib>
#include <iostream>

#include <boost/context/all.hpp>

typedef std::function<void(void*)> uthread_function;

class boost_uthread {
public:
    ~boost_uthread() 
    {
    }
    boost_uthread()
        : size_(boost::context::stack_traits::default_size())
    {
        this->stack_ = std::malloc(this->size_);
        this->fctx_ = boost::context::detail::make_fcontext(this->stack_, this->size_, &uthreadFunction);
    }

    void run(uthread_function callback, void* args)
    {
        this->callback_ = (callback);
        this->args_ = (args);
        // 得到协程ID，同uthreadFunction中代码，取回协程执行权.
        boost::context::detail::transfer_t t = boost::context::detail::jump_fcontext(this->fctx_, reinterpret_cast<void*>(this));
        // fprintf(stdout, "first jump_fcontext context:%p return:%p \n", this->fctx_, t.fctx);
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
        boost_uthread* ptr = reinterpret_cast<boost_uthread*>(t.data);
        // return run(),放出协程执行权.
        boost::context::detail::transfer_t t_ = boost::context::detail::jump_fcontext(t.fctx, nullptr);
        ptr->from_fctx_ = t_.fctx;
        ptr->callback_(ptr->args_);
    }

    boost::context::detail::fcontext_t fctx_;
    boost::context::detail::fcontext_t from_fctx_;

    uthread_function callback_;
    void* args_;
    void* stack_;
    std::size_t size_;
};

struct input_args {
    int target;
    int count_;
    bool quit_ = false;
    boost_uthread *c_;
};

void input_task(void* p)
{
    input_args* pia = (input_args*)p;

    while (true) {
        printf("number: ");
        int r = scanf("%d", &(pia->target));
        if (!r) {
            pia->quit_ = true;
            // return;
        }
        if (pia->target == 0) {
            pia->quit_ = true;
            // return;
        }
        pia->count_ = pia->count_ + 1;
        pia->c_->yield();
    }

    printf("ERROR: should not reach the end of input function\n");
}

int main()
{
    input_args ia = { 0, 0 };
    boost_uthread c1;
    ia.c_ = &c1;
    c1.run(&input_task, (void*)&ia);
    while (!ia.quit_) {
        c1.resume(); // 初始是协程让出控制权，现在是让协程取得使用权.
        // 第一次传fc_input，第二次把第一次传入的值写入，作为第二次重入的基础值.
        fprintf(stdout, "target=%d count=%d \n", ia.target, ia.count_);
    }

    printf("main: done\n");
    return 0;
}
