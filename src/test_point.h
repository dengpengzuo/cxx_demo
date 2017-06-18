#ifndef TEST_POINT_H
#define TEST_POINT_H
#include <stdlib.h>
#include <string.h>

namespace TestFd {
static const int row_size = 1024;
static const int col_size = 1024;

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
    if (!row) return NULL;
    return row[fd % col_size];
  }

 private:
  void **m_pp[row_size];  // 一个数组，数组的每一项都是指针的指针
                          // 数组内容：[void**][void**];
};
int function(void) {
  static int i, state = 0;
  switch (state) {
    case 0: /* start of function */
      fprintf(stdout, "case 0 \n");
      for (i = 0; i < 10; i++) {
        state = 1; /* so we will come back to "case 1" */
        return i;
        case 1:
          fprintf(stdout, "case 1 resume for for(i) loop \n"); /* resume control straight after the return */
      }
  }
}

int test_coroute() {
  int v = function();
  v = function();
  v = function();
  v = function();
  v = function();
}

void test_cls_fd() {
  clsFdMap fd;
  fd.set(0, 0);
  fd.set(1, 0);
}
};

#endif
