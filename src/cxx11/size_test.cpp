#include <gtest/gtest.h>

// 单元测试
TEST(FunTest, HandlesZeroInput) {
  EXPECT_EQ(4, sizeof(int));
  EXPECT_EQ(8, sizeof(long));
  EXPECT_EQ(8, sizeof(double));
}
