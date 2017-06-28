#include <cassert>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "life.h"

Life::Life(std::vector<Cell> initialState) {
  for (auto c : initialState) {
    toAdd.insert(c);
    toCheck.insert(c);
  }
}

void Life::step() {
  // Removals.
  for (auto c : toRemove) {
    alive.erase(c);
    updateNeighborCount(c, -1);
  }
  toRemove.clear();

  // Additions.
  for (auto c : toAdd) {
    alive.insert(c);
    updateNeighborCount(c, 1);
  }
  toAdd.clear();

  // Check possible additions or removals.
  for (Cell c : toCheck) {
    int n = neighbors[c];
    if (alive.find(c) != alive.end()) {
      if (n < 2 || n > 3) {
        toRemove.insert(c);
      }
    } else {
      if (n == 3) {
        toAdd.insert(c);
      }
    }
    // Purge uninteresting neighbors.
    if (n == 0) {
      neighbors.erase(c);
    }
  }
  toCheck.clear();
}

static long wrapAdd(long coord, int delta) {
  assert(delta == 0 || delta == 1 || delta == -1);
  if (coord == std::numeric_limits<long>::max() && delta > 0) {
    return std::numeric_limits<long>::min();
  } else if (coord == std::numeric_limits<long>::min() && delta < 0) {
    return std::numeric_limits<long>::max();
  } else {
    return coord + delta;
  }
}

void Life::updateNeighborCount(Cell cell, int delta) {
  for (auto d : directions) {
    long x = wrapAdd(cell.first, d.first);
    long y = wrapAdd(cell.second, d.second);
    Cell neighbor = Cell(x, y);
    neighbors[neighbor] += delta;
    toCheck.insert(neighbor);
  }
}

std::vector<Cell> Life::getAlive() const {
  // TODO(eworoshow): Copying the data is not ideal, but there is currently no good mechanism in
  // Emscripten to expose a const iterator or unordered_set to Javascript.
  std::vector<Cell> cells;
  cells.reserve(alive.size());
  for (auto c : alive) {
    cells.push_back(c);
  }
  return cells;
}

std::vector<Cell> parseCellList(std::string list) {
  std::vector<Cell> cells;

  std::stringstream slist(list);
  std::string line;
  while (std::getline(slist, line)) {
    long x, y;
    if (std::sscanf(line.c_str(), "(%ld, %ld)", &x, &y) == 2) {
      cells.push_back(Cell(x, y));
    }
  }

  return cells;
}
