#include <vector>
#include "life.h"
#include "gtest/gtest.h"
#include "gmock/gmock.h"

using ::testing::ElementsAre;
using ::testing::UnorderedElementsAre;

TEST(ParseCellList, InvalidInput) {
  EXPECT_TRUE(parseCellList("").empty());
  EXPECT_TRUE(parseCellList("()").empty());
  EXPECT_TRUE(parseCellList("(1, )").empty());
  EXPECT_TRUE(parseCellList("(1; 2)").empty());
  EXPECT_TRUE(parseCellList("(1, a)").empty());
  EXPECT_TRUE(parseCellList("(a, 1)").empty());
}

TEST(ParseCellList, ValidInput) {
  EXPECT_THAT(parseCellList("(1, 1)"), ElementsAre(Cell(1, 1)));
  EXPECT_THAT(parseCellList("(-2,2)"), ElementsAre(Cell(-2, 2)));
  EXPECT_THAT(parseCellList("(3, 3)\n()"), ElementsAre(Cell(3, 3)));
  EXPECT_THAT(parseCellList("(-4, 4)\n(a, b)\n( 5, -5 )\n"), ElementsAre(Cell(-4, 4), Cell(5, -5)));
}

TEST(AddAt, Idempotent) {
  Life life;
  life.addAt(1, 1);
  life.addAt(1, 1);
  life.addAt(-1, 2);
  life.step();
  EXPECT_THAT(life.getAlive(), UnorderedElementsAre(Cell(1, 1), Cell(-1, 2)));
}

TEST(Step, StaticPattern) {
  Life life;
  life.addAt(0, 1);
  life.addAt(1, 1);
  life.addAt(0, 0);
  life.addAt(1, 0);
  for (int i = 0; i < 100; i++) {
    life.step();
  }
  EXPECT_THAT(life.getAlive(), UnorderedElementsAre(Cell(0, 1), Cell(1, 1), Cell(0, 0), Cell(1, 0)));
}

TEST(Step, OscillatingPattern) {
  Life life;
  life.addAt(1, 2);
  life.addAt(1, 1);
  life.addAt(1, 0);
  for (int i = 0; i < 100; i++) {
    life.step();
    EXPECT_THAT(life.getAlive(), UnorderedElementsAre(Cell(1, 2), Cell(1, 1), Cell(1, 0)));
    life.step();
    EXPECT_THAT(life.getAlive(), UnorderedElementsAre(Cell(0, 1), Cell(1, 1), Cell(2, 1)));
  }
}

TEST(Step, TranslatePattern) {
  Life life;
  life.addAt(0, 2);
  life.addAt(1, 1);
  life.addAt(2, 1);
  life.addAt(0, 0);
  life.addAt(1, 0);
  for (int i = 0; i < 100; i++) {
    life.step();
    EXPECT_THAT(
        life.getAlive(),
        UnorderedElementsAre(Cell(0+i, 2-i), Cell(1+i, 1-i), Cell(2+i, 1-i), Cell(0+i, 0-i), Cell(1+i, 0-i)));
    life.step();
    EXPECT_THAT(
        life.getAlive(),
        UnorderedElementsAre(Cell(0+i, 0-i), Cell(1+i, 0-i), Cell(2+i, 0-i), Cell(1+i, 2-i), Cell(2+i, 1-i)));
    life.step();
    EXPECT_THAT(
        life.getAlive(),
        UnorderedElementsAre(Cell(1+i, 0-i), Cell(2+i, 0-i), Cell(0+i, 1-i), Cell(2+i, 1-i), Cell(1+i, -1-i)));
    life.step();
    EXPECT_THAT(
        life.getAlive(),
        UnorderedElementsAre(Cell(0+i, 0-i), Cell(2+i, 0-i), Cell(1+i, -1-i), Cell(2+i, -1-i), Cell(2+i, 1-i)));
  }
}

TEST(Step, DisappearingPattern) {
  Life life;
  life.addAt(6, 2);
  life.addAt(0, 1);
  life.addAt(1, 1);
  life.addAt(1, 0);
  life.addAt(5, 0);
  life.addAt(6, 0);
  life.addAt(7, 0);
  for (int i = 0; i <= 130; i++) { // Diehard lives for 130 generations.
    life.step();
  }
  EXPECT_TRUE(life.getAlive().empty());
}

TEST(Step, NumericLimits) {
  long min = std::numeric_limits<long>::min();
  long max = std::numeric_limits<long>::max();

  Life life;
  life.addAt(max, max);
  life.addAt(min, max);
  life.addAt(max, min);
  life.addAt(min, min);
  for (int i = 0; i < 100; i++) {
    life.step();
  }
  EXPECT_THAT(life.getAlive(), UnorderedElementsAre(Cell(max, max), Cell(min, max), Cell(max, min), Cell(min, min)));
}
