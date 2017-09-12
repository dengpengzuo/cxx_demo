template<typename Alloc>
boost_uthread<Alloc>::boost_uthread()
        : alloc_() {
    this->stack_ = this->alloc_.allocate();
}

template<typename Alloc>
boost_uthread<Alloc>::~boost_uthread() {
    this->alloc_.deallocate(this->stack_);
}

template<typename Alloc>
void boost_uthread<Alloc>::run(uthread_functionx callback, void *args) {
    this->callback_ = (callback);
    this->args_ = (args);
    // 得到协程ID，同uthreadFunction中代码，取回协程执行权.
    boost::context::detail::fcontext_t fctx = boost::context::detail::make_fcontext(this->stack_.sp, this->stack_.size,
                                                &boost_uthread<Alloc>::uthreadEntry);
    boost::context::detail::transfer_t t = boost::context::detail::jump_fcontext(fctx, reinterpret_cast<void *>(this));
    this->fctx_ = t.fctx;
}

template<typename Alloc>
void boost_uthread<Alloc>::resume() {
    // 将执行权给协程.
    boost::context::detail::transfer_t t = boost::context::detail::jump_fcontext(this->fctx_, nullptr);
    // fprintf(stdout, "resume jump fcontext:%p return: %p\n", this->fctx_, t.fctx);
    this->fctx_ = t.fctx;
}

template<typename Alloc>
void boost_uthread<Alloc>::yield() {
    // 协程让出执行权
    boost::context::detail::transfer_t t = boost::context::detail::jump_fcontext(this->from_fctx_, nullptr);
    // fprintf(stdout, "yield jump fcontext:%p return: %p\n", this->from_fctx_, t.fctx);
    this->from_fctx_ = t.fctx;
}

template<typename Alloc>
void boost_uthread<Alloc>::uthreadEntry(boost::context::detail::transfer_t t) {
    boost_uthread<Alloc> *ptr = static_cast<boost_uthread<Alloc> *>(t.data);
    // return run(),放出协程执行权.
    boost::context::detail::transfer_t t_ = boost::context::detail::jump_fcontext(t.fctx, nullptr);
    ptr->from_fctx_ = t_.fctx;
    ptr->callback_(ptr, ptr->args_);
}
