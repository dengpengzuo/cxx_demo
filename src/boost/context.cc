

#include <cstdio>
#include <cstdlib>
#include <iostream>

#include <boost/context/all.hpp>

typedef std::function<void(void*)> uthread_function;

class boost_uthread {
 public:
  ~boost_uthread() {}
  boost_uthread()
      : size_(boost::context::stack_traits::default_size()) {
    this->stack_ = std::malloc(this->size_);
    this->fctx_ = boost::context::detail::make_fcontext(this->stack_, this->size_, &uthreadFunction);
  }

  void run(uthread_function callback, void* args) {
    this->callback_ = (callback);
    this->args_ = (args);
    fprintf(stdout,"run fcontext:%p begin.\n", this->fctx_);
    boost::context::detail::transfer_t t = boost::context::detail::jump_fcontext(this->fctx_, reinterpret_cast<void*>(this));
    this->fctx_ = t.fctx;
    fprintf(stdout,"run change fcontext:%p end.\n", this->fctx_);
  }

  void resume() { 
    fprintf(stdout,"resume jump fcontext:%p \n", this->fctx_);
    boost::context::detail::transfer_t t = boost::context::detail::jump_fcontext(this->fctx_, nullptr); 
    fprintf(stdout,"resume return:\n", this->fctx_);
  }

  void yield() {
    fprintf(stdout, "yield jump fcontext:%p \n", this->from_fctx_);
    boost::context::detail::jump_fcontext(this->from_fctx_, nullptr);
  }

 private:
  static void uthreadFunction(boost::context::detail::transfer_t t) {
    boost_uthread* ptr = reinterpret_cast<boost_uthread*>(t.data);
    ptr->from_fctx_ = t.fctx;
    fprintf(stdout, "uthreadFunction run fromfctx: %p \n", t.fctx);
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
};

void input_task(void* p) ;

bool b_quit = false;
input_args ia = {0,0};
boost_uthread c1;

extern void input_task(void* p) {
  input_args* pia = (input_args*)p;

  while (true) {
    printf("number: ");
    int r = scanf("%d", &(pia->target));
    if (!r) {
      b_quit = true;
      // return;
    }
    if (pia->target == 0) {
      b_quit = true;
      // return;
    }
    pia->count_ = pia->count_ + 1;
    c1.yield();
  }

  printf("ERROR: should not reach the end of input function\n");
}

int main() {
  c1.run(&input_task, (void*)&ia);
  while (!b_quit) {
    // 第一次传fc_input，第二次把第一次传入的值写入，作为第二次重入的基础值.
    printf("target=%d count=%d \n", ia.target, ia.count_);
    c1.resume();
  }

  printf("main: done\n");
  return 0;
}
