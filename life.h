#ifndef LIFE_H
#define LIFE_H

#include <functional>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

// NB: Should be int64_t but Javascript has no native support for 64-bit integers and so
// Emscripten/Embind correspondingly has no Javascript representation for int64_t.
// TODO(eworoshow): Parameterize the numeric type.
typedef std::pair<long, long> Cell;

struct CellHash {
  std::size_t operator() (const Cell& c) const {
    size_t h = 17;
    h = h * 31 + std::hash<long>()(c.first);
    h = h * 31 + std::hash<long>()(c.second);
    return h;
  }
};

class Life {
public:
  // Starts a new simulation with the specified cells starting as alive.
  Life(std::vector<Cell> initialState);

  // Increments the simulation one step.
  void step();

  // Returns a copy of the cells currently alive.
  std::vector<Cell> getAlive() const;

private:
  void updateNeighborCount(Cell cell, int delta);

  std::unordered_map<Cell, int, CellHash> neighbors;

  std::unordered_set<Cell, CellHash> alive;
  std::unordered_set<Cell, CellHash> toRemove;
  std::unordered_set<Cell, CellHash> toAdd;
  std::unordered_set<Cell, CellHash> toCheck;

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
