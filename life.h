#ifndef LIFE_H
#define LIFE_H

#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <unordered_set>
#include <vector>

// NB: Should be int64_t but Javascript has no native support for 64-bit integers and so
// Emscripten/Embind correspondingly has no Javascript representation for int64_t.
// TODO(eworoshow): Parameterize the numeric type.
typedef std::pair<long, long> Coord;

struct Cell {
  long x;
  long y;

  int liveNeighbors;

  Cell() : x(0), y(0), liveNeighbors(0) {}
  Cell(long x, long y) : x(x), y(y), liveNeighbors(0) {}
};

bool operator==(const Cell& lhs, const Cell& rhs);
std::ostream& operator<<(std::ostream& stream, Cell const& cell);

struct CellHash {
  std::size_t operator() (std::shared_ptr<Cell> const &c) const {
    size_t h = 17;
    h = h * 31 + std::hash<long>()(c->x);
    h = h * 31 + std::hash<long>()(c->y);
    return h;
  }
};

struct CellCompare {
  size_t operator() (std::shared_ptr<Cell> const &lhs, std::shared_ptr<Cell> const &rhs) const {
    return *lhs == *rhs;
  }
};

class Life {
public:
  Life() {}

  // Triggers a cell to be added at the specified location in the next step.
  void addAt(long x, long y);

  // Increments the simulation one step.
  void step();

  // Returns a copy of the cells currently alive.
  std::vector<Cell> getAlive() const;

private:
  void updateNeighborCount(std::shared_ptr<Cell> cell, int delta);

  std::unordered_set<std::shared_ptr<Cell>, CellHash, CellCompare> all;

  std::unordered_set<std::shared_ptr<Cell>, CellHash, CellCompare> alive;
  std::unordered_set<std::shared_ptr<Cell>, CellHash, CellCompare> toRemove;
  std::unordered_set<std::shared_ptr<Cell>, CellHash, CellCompare> toAdd;
  std::unordered_set<std::shared_ptr<Cell>, CellHash, CellCompare> toCheck;

  const std::vector<std::pair<int, int>> directions = {
    {0, 1},
    {1, 1},
    {1, 0},
    {1, -1},
    {0, -1},
    {-1, -1},
    {-1, 0},
    {-1, 1},
  };
};

// Parses a newline-delimited set of (x, y) cells. Invalid input is skipped.
std::vector<Cell> parseCellList(std::string list);

#endif
