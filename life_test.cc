#include <vector>
#include "life.h"
#include "gtest/gtest.h"
#include "gmock/gmock.h"

using ::testing::ElementsAre;

TEST(ParseCellList, InvalidInput) {
  EXPECT_TRUE(parseCellList("").empty());
  EXPECT_TRUE(parseCellList("()").empty());
  EXPECT_TRUE(parseCellList("(1, )").empty());
  EXPECT_TRUE(parseCellList("(1; 2)").empty());
  EXPECT_TRUE(parseCellList("(1, a)").empty());
  EXPECT_TRUE(parseCellList("(a, 1)").empty());
}

TEST(ParseCellList, ValidInput) {
  EXPECT_THAT(parseCellList("(1, 1)"), ElementsAre((Cell) {1, 1}));
  EXPECT_THAT(parseCellList("(-2,2)"), ElementsAre((Cell) {-2, 2}));
  EXPECT_THAT(parseCellList("(3, 3)\n()"), ElementsAre((Cell) {3, 3}));
  EXPECT_THAT(parseCellList("(-4, 4)\n(a, b)\n( 5, -5 )\n"), ElementsAre((Cell) {-4, 4}, (Cell) {5, -5}));
}

