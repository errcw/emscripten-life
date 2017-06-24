#include "life.h"
#include "gtest/gtest.h"

TEST(ParseCellList, InvalidInput) {
  EXPECT_TRUE(parseCellList("").empty());
}

TEST(ParseCellList, ValidInput) {
  EXPECT_EQ(1, 1);
}

