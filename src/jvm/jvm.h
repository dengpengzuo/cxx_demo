#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <new>

#if __GNUC__ < 3 // gcc 2.x does not support noinline attribute
#define _NOINLINE_
#define _AINLIDE_
#else
#define _NOINLINE_ __attribute__((noinline))
#define _AINLIDE_ __attribute__((always_inline))
#endif

/*
 * Memory types
 */
enum MemoryType {
  // Memory type by sub systems. It occupies lower byte.
  mtJavaHeap = 0x00, // Java heap
  mtClass = 0x01,    // memory class for Java classes
  mtThread = 0x02,   // memory for thread objects
  mtThreadStack = 0x03,
  mtCode = 0x04,            // memory for generated code
  mtGC = 0x05,              // memory for GC
  mtCompiler = 0x06,        // memory for compiler
  mtInternal = 0x07,        // memory used by VM, but does not belong to
                            // any of above categories, and not used for
                            // native memory tracking
  mtOther = 0x08,           // memory not used by VM
  mtSymbol = 0x09,          // symbol
  mtNMT = 0x0A,             // memory used by native memory tracking
  mtClassShared = 0x0B,     // class data sharing
  mtChunk = 0x0C,           // chunk that holds content of arenas
  mtTest = 0x0D,            // Test type for verifying NMT
  mtTracing = 0x0E,         // memory used for Tracing
  mtNone = 0x0F,            // undefined
  mt_number_of_types = 0x10 // number of memory types (mtDontTrack
                            // is not included as validate type)
};

typedef MemoryType MEMFLAGS;

class AllocFailStrategy {
public:
  enum AllocFailEnum { EXIT_OOM, RETURN_NULL };
};

typedef AllocFailStrategy::AllocFailEnum AllocFailType;

template <MEMFLAGS F> class CHeapObj {
public:
  _NOINLINE_ void *operator new(size_t size) throw();

  _NOINLINE_ void *operator new(size_t size,
                                const std::nothrow_t &nothrow_constant) throw();

  _NOINLINE_ void *operator new[](size_t size) throw();

  _NOINLINE_ void *
  operator new[](size_t size, const std::nothrow_t &nothrow_constant) throw();

  void operator delete(void *p);

  void operator delete[](void *p);
};

_AINLIDE_ inline void FreeHeap(void *p, MEMFLAGS memflags = mtInternal) {
  free(p);
}

_AINLIDE_ inline char *
AllocateHeap(size_t size, MEMFLAGS flags,
             AllocFailType alloc_failmode = AllocFailStrategy::EXIT_OOM) {
  char *p = (char *)malloc(size);
  if (p == NULL && alloc_failmode == AllocFailStrategy::EXIT_OOM) {
    // vm_exit_out_of_memory(size, "AllocateHeap");
  }
  return p;
}

template <MEMFLAGS F> void *CHeapObj<F>::operator new(size_t size) throw() {
  void *p = (void *)AllocateHeap(size, F);
  return p;
}

template <MEMFLAGS F>
void *CHeapObj<F>::
operator new(size_t size, const std::nothrow_t &nothrow_constant) throw() {
  void *p = (void *)AllocateHeap(size, F, AllocFailStrategy::RETURN_NULL);
  return p;
}

template <MEMFLAGS F> void *CHeapObj<F>::operator new[](size_t size) throw() {
  return CHeapObj<F>::operator new(size);
}

template <MEMFLAGS F>
void *CHeapObj<F>::
operator new[](size_t size, const std::nothrow_t &nothrow_constant) throw() {
  return CHeapObj<F>::operator new(size, nothrow_constant);
}

template <MEMFLAGS F> void CHeapObj<F>::operator delete(void *p) {
  FreeHeap(p, F);
}

template <MEMFLAGS F> void CHeapObj<F>::operator delete[](void *p) {
  FreeHeap(p, F);
}
