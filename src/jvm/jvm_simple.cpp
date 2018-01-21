#include <iostream>

extern bool UseCompressedClassPointers;

#include "oop.hpp"

int main(int argc, char **argv) {
    // 64 位 初始是压缩对象指针.
    UseCompressedClassPointers = true;

    // http://openjdk.java.net/groups/hotspot/docs/StorageManagement.html
    // https://code.google.com/archive/p/strongtalk/wikis/VMTypesForSmalltalkObjects.wiki
    // 在Java程序运行的过程中，每创建一个新的对象，在JVM内部就会相应地创建一个对应类型的oop对象。
    // 各种oop类的共同基类为 oopDesc 类。
    instanceOopDesc inst;
    inst.set_mark(nullptr); // 内存地址

    // 对象头的各种操作方法封装.
    markOopDesc *p = inst.mark();
    uintptr_t v = p->value();
    (void)v;

    UseCompressedClassPointers = false;
    return EXIT_SUCCESS;
}
