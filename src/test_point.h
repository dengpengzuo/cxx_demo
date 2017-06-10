#ifndef TEST_POINT_H
#define TEST_POINT_H
#include <stdlib.h>
#include <string.h>

class clsFdMap {
public:
  static const int row_size = 1024;
  static const int col_size = 1024;
  void **m_pp[row_size]; // 一个数组，数组的每一项都是指针的指针
  // 数组内容：[void**][void**];

  clsFdMap() { memset(m_pp, 0, sizeof(m_pp)); }
  ~clsFdMap() {
    for (int i = 0; i < sizeof(m_pp) / sizeof(m_pp[0]); i++) {
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
    m_pp[idx][fd % col_size] = (void *)ptr;
    return 0;
  }
  inline void *get(int fd) {
    int idx = fd / row_size;
    if (idx < 0 || idx >= row_size) {
      return NULL;
    }
    void ** row = m_pp[idx];
    return row [fd % col_size];
  }
};

#endif
