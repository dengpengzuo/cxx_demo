
#ifndef TEST_2ARRAY_H
#define TEST_2ARRAY_H

#include <iostream>
#include <sstream>
#include <time.h>

namespace Test_2Array {
#define PRINT_ARRAY(AR, LEN)                        \
    do {                                            \
        std::ostringstream buf;                     \
        for (int __i = 0; __i < LEN; ++__i) {       \
            buf << "\n[" << __i << "]=" << AR[__i]; \
        }                                           \
        std::cout << buf.str() << std::endl;        \
    } while(0)
//------------------------------------------------------------------------------
// 微信面试找出次数
    static void
    print_2array_wx() {
        int array_wx[] = {1, 2, 3, 3, 3, 4, 2};
        int len = sizeof(array_wx) / sizeof(array_wx[0]);
        int num = 0;
        int count = 0;

        for (int i = 0; i < len; ++i) {
            if (count == 0) {
                num = array_wx[i];
                count = 1;
            } else {
                if (num == array_wx[i])
                    ++count;
                else
                    --count;
            }
        }
        if (count > 0) {
            count = (len - count) / 2 + count;
            PRINT_ARRAY(array_wx, len);
            fprintf(stdout, "发现出现次数过半数字：%d, 出现次数：%d\n", num, count);
        }
    }

//------------------------------------------------------------------------------
// 双打对角线方式打印
    static void
    print_2array_impl() {
        typedef int (*array4_ptr)[4]; // 指向2维数组的镄针
        int v[][4] = {
                {1,  2,  3,  4},
                {5,  6,  7,  8},
                {9,  10, 11, 12},
                {13, 14, 15, 16}
        };
        array4_ptr p = &v[0];
        int n = 4;
        std::ostringstream buf;
        // 右对角线
        for (int i = n - 1; i >= 0; --i) {
            array4_ptr t = p;
            for (int j = 0, c = i; j < n && c < n; ++j) {
                buf << (*t)[c] << ","; // (*t)[e] ==> t指针的第e列 ，*t[e] ==> t指针(+e) 所指的值.
                // 下一行，下一个位置。
                ++t;
                ++c;
            }
        }
        // 左对角线
        for (int i = 1; i < n; ++i) {
            array4_ptr t = p + i;
            for (int j = i, c = 0; j < n && c < n; ++j) {
                buf << (*t)[c] << ",";
                // 下一行，下一个位置。
                ++t;
                ++c;
            }
        }
        std::cout << buf.str() << std::endl;
    }

//------------------------------------------------------------------------------
// 二发查找[同时tkm向前，向后找了的]
// right = n-1 => while(left <= right) => right = middle-1;
    static void
    find_array_result(int *array, int len, int v, int except) {
        int low = 0;
        int high = len - 1;
        int mid = 0;
        while (low <= high) {
            mid = low + ((high - low) >> 1);  //防止溢出，移位也更高效。同时，每次循环都需要更新。
            if (array[mid] > v) {
                high = mid - 1;
            } else if (array[mid] < v) {
                low = mid + 1;
            } else {
                int i = mid - 1, j = mid + 1;
                for (; i >= 0 && array[i] == v; --i);
                for (; j < len && array[j] == v; ++j);
                if (++i == --j)
                    std::cout << "find val:" << v << " at:[" << mid << "]" << std::endl;
                else
                    std::cout << "find val:" << v << " at:[" << i << "," << j << "]" << std::endl;
                break;
            }
        }
    }

    // 插入排序
    static void
    insert_sort_array(int *a, int len) {
        int right = len - 1; // right 必须是len-1
        for (int i = 0, j = i; i < right; j = ++i) {
            int ai = a[i + 1]; // 直接取下一个数组值.

            while (ai < a[j]) {
                a[j + 1] = a[j]; // a[j]后移到a[j+1]
                --j;
                if (j < 0) {     // **注意必须是到-1.
                    break;
                }
            }
            a[j + 1] = ai;
        }
    }

    static double
    random(double start, double end) {
        return start + (end - start) * ::rand() / (RAND_MAX + 1.0);
    }

    static void
    print_2find_array() {
        int array_int[128];
        int len = sizeof(array_int) / sizeof(array_int[0]);
        ::srand(::time(0));
        for (int i = 0; i < len; ++i) {
            array_int[i] = int(random(0, 10000));
        }

        PRINT_ARRAY(array_int, len);
        insert_sort_array(&array_int[0], len); // 小数组使用插入排序.
        PRINT_ARRAY(array_int, len);

        for (int i = 0; i < len; ++i) {
            find_array_result(&array_int[0], len, array_int[i], i);
        }
    }

    void array_main() {
        print_2array_impl();
        print_2array_wx();
        print_2find_array();
    }
};
#endif // TEST_2ARRAY_H