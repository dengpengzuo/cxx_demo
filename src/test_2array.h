
#ifndef TEST_2ARRAY_H
#define TEST_2ARRAY_H

#include <iostream>
#include <sstream>

namespace Test_2Array {
int v[][4] = {{1, 2, 3, 4}, {5, 6, 7, 8}, {9, 10, 11, 12}, {13, 14, 15, 16}};
int array_wx[] = {1, 2, 3, 2, 2};

int array_int[500] = {0};

typedef int (*array4_ptr)[4];  // 指向2维数组的镄针

// 微信面试找出次数
static void print_2array_wx() {
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
    fprintf(stdout, "发现出现次数过半数字：%d, 出现次数：%d\n", num, count);
  }
}
// 双打对角线方式打印
static void print_2array_impl(array4_ptr p, int n) {
  std::ostringstream buf;
  // 右对角线
  for (int i = n - 1; i >= 0; --i) {
    array4_ptr t = p;
    for (int j = 0, c = i; j < n && c < n; ++j) {
      buf << (*t)[c] << ",";  // (*t)[e] ==> t指针的第e列 ，*t[e] ==> t指针(+e) 所指的值.
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

static void find_array_result(int *array, int len, int v) {
  int low = 0;
  int high = len - 1;
  std::ostringstream buf;
  while (low <= high) { // 因为 high = len-1 所以这儿要 <=
    buf << "find [" << low << "," << high << "),";
    
    int mid = (low + high) >> 1 ; // low + (high - low) / 2

    if (array[mid] == v) {
      buf << "find result:" << v << " at " << mid << ".\n";
      break;
    } else if (array[mid] > v) {
      high = mid;  // 这儿必须是 mid , 因为 high = len - 1;
    } else {      // array[mid] < v
      low = mid + 1;
    }
  }
  std::cout << buf.str();
}

// 二分查找法
static void print_2find_array() {
  int len = sizeof(array_int) / sizeof(array_int[0]);
  for (int i = 0; i < len ; ++i) array_int[i] = i;
  for (int i = 0; i < len; ++i) {
    find_array_result(&array_int[0], len, i);
  }
}

void print_2array() {
  print_2array_impl(&v[0], 4);
  print_2array_wx();
  print_2find_array();
}
};
#endif  // TEST_2ARRAY_H