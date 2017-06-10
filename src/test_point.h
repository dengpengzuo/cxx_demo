#ifndef TEST_POINT_H
#define TEST_POINT_H
#include <stdlib.h>
#include <string.h>

namespace TestFd {
static const int row_size = 1024;
static const int col_size = 1024;
struct cFdMap {
  void **array[row_size];
};

class clsFdMap {
public:
  clsFdMap() { memset(m_pp, 0, sizeof(m_pp)); }
  ~clsFdMap() {
    for (int i = 0; i < row_size; i++) {
      if (m_pp[i]) {
        delete[] m_pp[i];
        m_pp[i] = NULL;
      }
    }
  }
  inline int clear(int fd) {
    set(fd, NULL);
    return 0;
  }
  inline int set(int fd, const void *ptr) {
    int idx = fd / row_size;
    if (idx < 0 || idx >= row_size) {
      return -1;
    }
    if (!m_pp[idx]) {
      m_pp[idx] = new void *[col_size];
    }
    void **row = m_pp[idx];
    row[fd % col_size] = (void *)ptr;
    return 0;
  }
  inline void *get(int fd) {
    int idx = fd / row_size;
    if (idx < 0 || idx >= row_size) {
      return NULL;
    }
    void **row = m_pp[idx];
    if (!row)
      return NULL;
    return row[fd % col_size];
  }

private:
  void **m_pp[row_size]; // 一个数组，数组的每一项都是指针的指针
                         // 数组内容：[void**][void**];
};

void test_cls_fd() {
  clsFdMap fd;
  fd.set(0, 0);
  fd.set(1, 0);
}
};

#endif
