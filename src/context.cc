#include <boost/context/all.hpp>

boost::context::detail::fcontext_t fc_avg, fc_input;

bool b_quit = false;

// --- The Average Task ---
// Computes the average of the input data and
// then yields execution back to the main task
//-------------------------
// struct average_args - data for the task
// average_yield() - wrapper for yielding execution back to the main task
// average_task() - the task logic
// ------------------------
struct average_args {
  int* source;
  int sum;
  int count;
  int average;
  int task_;
};

void average_yield(boost::context::detail::transfer_t d) {
  boost::context::detail::jump_fcontext(d.fctx, nullptr);
}

void average_task(boost::context::detail::transfer_t p) {
  average_args* args = (average_args*)p.data;
  args->sum = 0;
  args->count = 0;
  args->average = 0;
  while (true) {
    args->sum += *args->source;
    ++args->count;
    args->average = args->sum / args->count;
    average_yield(p);
  }

  printf("ERROR: should not reach the end of average function\n");
}

// --- The Input Task ---
// Reads a number as input from the console and
// then yields execution back to the main task
// ----------------------
// struct input_args - data for the task
// input_yield() - wrapper for yielding execution back to the main task
// input_task() - the task logic
// ----------------------
struct input_args {
  average_args* aa;
  int* target;
  int task_;
};
void input_yield(boost::context::detail::transfer_t t) {
  boost::context::detail::jump_fcontext(t.fctx, nullptr);
}
void input_task(boost::context::detail::transfer_t p) {
  input_args* pia = (input_args*)p.data;

  while (true) {
    printf("number: ");
    int r = scanf("%d", pia->target);
    if (!r) {
      b_quit = true;
      // return;
    }
    if (*(pia->target) == 0) {
      b_quit = true;
      // return;
    }

    input_yield(p);  // tranfer_t 中是从那个fcontext_t来的.
  }

  printf("ERROR: should not reach the end of input function\n");
}

int main() {
  int share = -1;
  
  average_args aa = {&share};
  input_args ia = {&aa, &share};

  std::size_t size(boost::context::stack_traits::default_size());
  void* fc_avg_base(std::malloc(size));
  void* fc_input_base(std::malloc(size));

  // construct the input task
  fc_input =
      boost::context::detail::make_fcontext(fc_input_base, size, input_task);

  // construct the average task
  fc_avg =
      boost::context::detail::make_fcontext(fc_avg_base, size, average_task);
  //
  boost::context::detail::transfer_t it{fc_input, nullptr}, at{fc_avg, nullptr};
  while (!b_quit) {
    // 第一次传fc_input，第二次把第一次传入的值写入，作为第二次重入的基础值.
    it = boost::context::detail::jump_fcontext(it.fctx, (void*)&ia);
    at = boost::context::detail::jump_fcontext(at.fctx, (void*)&aa);
    printf("sum=%d count=%d average=%d\n", aa.sum, aa.count, aa.average);
  }

  printf("main: done\n");
  return 0;
}
