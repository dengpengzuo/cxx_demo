#include <functional>
#include <boost/context/all.hpp>

template <typename Alloc>
class boost_uthread {
public:
    typedef std::function<void(boost_uthread<Alloc>*, void*)> uthread_functionx;

    ~boost_uthread();

    boost_uthread();

    void run(uthread_functionx callback, void* args);

    void resume();

    void yield();

private:
    static void uthreadEntry(boost::context::detail::transfer_t t);

    boost::context::detail::fcontext_t fctx_;
    boost::context::detail::fcontext_t from_fctx_;

    uthread_functionx callback_;
    void* args_;

    Alloc alloc_;
    boost::context::stack_context stack_;
};

#include "uthread.inl"

typedef boost_uthread< boost::context::protected_fixedsize_stack > pfx_ucthread;