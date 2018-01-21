#ifndef CXX_DEMO_OOP_HPP_H
#define CXX_DEMO_OOP_HPP_H

#include <cstdint>

// 64 位 初始是压缩对象指针.
bool UseCompressedClassPointers;

class HeapWord {
private:
    char *i;
};

typedef uint32_t juint;
typedef juint narrowKlass;

const int HeapWordSize = sizeof(HeapWord); // 8
const int LogHeapWordSize = 3;
const int LogBytesPerLong = 3;
const int LogBytesPerWord = 3;
const int LogBitsPerByte = 3;

const int BytesPerLong = 1 << LogBytesPerLong;
const int LogBitsPerWord = LogBitsPerByte + LogBytesPerWord;
const int BitsPerWord = 1 << LogBitsPerWord;

const int HeapWordsPerLong = BytesPerLong / HeapWordSize;
const int LogHeapWordsPerLong = LogBytesPerLong - LogHeapWordSize;


const int LogKlassAlignmentInBytes = 3;
const int LogKlassAlignment = LogKlassAlignmentInBytes - LogHeapWordSize;
const int KlassAlignmentInBytes = 1 << LogKlassAlignmentInBytes;
const int KlassAlignment = KlassAlignmentInBytes / HeapWordSize;

class oopDesc;

class markOopDesc;

class instanceOopDesc;


typedef class markOopDesc *markOop;
typedef class oopDesc *oop;
typedef class instanceOopDesc *instanceOop;

// Pointer subtraction.
// The idea here is to avoid ptrdiff_t, which is signed and so doesn't have
// the range we might need to find differences from one end of the heap
// to the other.
// A typical use might be:
//     if (pointer_delta(end(), top()) >= size) {
//       // enough room for an object of size
//       ...
// and then additions like
//       ... top() + size ...
// are safe because we know that top() is at least size below end().
inline size_t pointer_delta(const void *left,
                            const void *right,
                            size_t element_size) {
    return (((uintptr_t) left) - ((uintptr_t) right)) / element_size;
}

//------------------------------------------------------------------------------------------------------------------
typedef u_char *address;
struct NarrowPtrStruct {
    // Base address for oop-within-java-object materialization.
    // NULL if using wide oops or zero based narrow oops.
    address _base;
    // Number of shift bits for encoding/decoding narrow ptrs.
    // 0 if using wide ptrs or zero based unscaled narrow ptrs,
    // LogMinObjAlignmentInBytes/LogKlassAlignmentInBytes otherwise.
    int _shift;
    // Generate code with implicit null checks for narrow ptrs.
    bool _use_implicit_null_checks;
};

static struct NarrowPtrStruct _narrow_klass;

class Universe {
public:
    static int narrow_klass_shift() { return _narrow_klass._shift; }

    static address narrow_klass_base() { return _narrow_klass._base; }
};

class Metadata {
public:
};

class Klass : public Metadata {
private:
public:

    static bool is_null(narrowKlass obj);

    static bool is_null(Klass *obj);

    // klass encoding for klass pointer in objects.
    static narrowKlass encode_klass_not_null(Klass *v);

    static narrowKlass encode_klass(Klass *v);

    static Klass *decode_klass_not_null(narrowKlass v);

    static Klass *decode_klass(narrowKlass v);
};

inline bool Klass::is_null(Klass *obj) { return obj == NULL; }

inline bool Klass::is_null(narrowKlass obj) { return obj == 0; }

// Encoding and decoding for klass field.
inline bool check_klass_alignment(Klass *obj) {
    return (intptr_t) obj % KlassAlignmentInBytes == 0;
}

inline narrowKlass Klass::encode_klass_not_null(Klass *v) {
    int shift = Universe::narrow_klass_shift();
    uint64_t pd = (uint64_t) (pointer_delta((void *) v, Universe::narrow_klass_base(), 1));
    uint64_t result = pd >> shift;
    return (narrowKlass) result;
}

inline narrowKlass Klass::encode_klass(Klass *v) {
    return is_null(v) ? (narrowKlass) 0 : encode_klass_not_null(v);
}

inline Klass *Klass::decode_klass_not_null(narrowKlass v) {
    int shift = Universe::narrow_klass_shift();
    Klass *result = (Klass *) (void *) ((uintptr_t) Universe::narrow_klass_base() + ((uintptr_t) v << shift));
    return result;
}

inline Klass *Klass::decode_klass(narrowKlass v) {
    return is_null(v) ? (Klass *) NULL : decode_klass_not_null(v);
}

class InstanceKlass : public Klass {
public:
    // See "The Java Virtual Machine Specification" section 2.16.2-5 for a detailed description
    // of the class loading & initialization procedure, and the use of the states.
    enum ClassState {
        allocated,                          // allocated (but not yet linked)
        loaded,                             // loaded and inserted in class hierarchy (but not linked yet)
        linked,                             // successfully linked/verified (but not initialized yet)
        being_initialized,                  // currently running class initializer
        fully_initialized,                  // initialized (successfull final state)
        initialization_error                // error happened during initialization
    };
};

class ArrayKlass : public Klass {
public:

};

//------------------------------------------------------------------------------------------------------------------
// jvm对象描述
class oopDesc {
private:
    volatile markOop _mark;
    union _metadata {
        Klass *_klass;
        narrowKlass _compressed_klass;
    } _metadata;
public:
    // 接收外部的对象实例的指针地址.
    void set_mark(volatile markOop m) { _mark = m; }

    markOop mark() const { return _mark; }

    void set_klass(Klass *k) {
        if (UseCompressedClassPointers) {
            _metadata._compressed_klass = Klass::encode_klass_not_null(k);
        } else {
            _metadata._klass = k;
        }
    }

    Klass *klass() const {
        if (UseCompressedClassPointers) {
            return Klass::decode_klass(_metadata._compressed_klass);
        } else {
            return _metadata._klass;
        }
    }
};

// 封装了读取[oopDesc对象头也就是_mark的常用操作]
class markOopDesc : public oopDesc {
public:
    uintptr_t value() const { return (uintptr_t) this; }
    // 比如取[第1位.......]，对象锁等
    // Constants
    enum {
        age_bits = 4,
        lock_bits = 2,
        biased_lock_bits = 1,
        max_hash_bits = BitsPerWord - age_bits - lock_bits - biased_lock_bits,
        hash_bits = max_hash_bits > 31 ? 31 : max_hash_bits,
        cms_bits = 1, //
        epoch_bits = 2
    };

    enum {
        locked_value = 0,
        unlocked_value = 1,
        monitor_value = 2,
        marked_value = 3,
        biased_lock_pattern = 5
    };
};

// 实例对象
class instanceOopDesc : public oopDesc {
public:
    // aligned header size.
    static int header_size() { return sizeof(instanceOopDesc) / HeapWordSize; }
};

class arrayOopDesc : public oopDesc {
public:
};
//------------------------------------------------------------------------------------------------------------------



#endif //CXX_DEMO_OOP_HPP_H
