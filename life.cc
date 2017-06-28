#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include "life.h"

bool operator==(const Cell& lhs, const Cell& rhs) {
  return lhs.x == rhs.x && lhs.y == rhs.y;
}

std::ostream& operator<<(std::ostream& stream, Cell const& cell) {
  return stream << "(" << cell.x << "," << cell.y << ")" << cell.liveNeighbors;
}

void Life::addAt(long x, long y) {
  auto cell = std::make_shared<Cell>(x, y);
  if (alive.find(cell) != alive.end()) {
    return;
  }
  all.insert(cell);
  toAdd.insert(cell);
  toCheck.insert(cell);
  toRemove.erase(cell);
}

void Life::step() {
  // Removals.
  for (auto i : toRemove) {
    alive.erase(i);
    updateNeighborCount(i, -1);
  }
  toRemove.clear();

  // Additions.
  for (auto i : toAdd) {
    alive.insert(i);
    updateNeighborCount(i, 1);
  }
  toAdd.clear();

  // Check possible additions or removals.
  for (auto i : toCheck) {
    if (alive.find(i) != alive.end()) {
      if (i->liveNeighbors < 2 || i->liveNeighbors > 3) {
        toRemove.insert(i);
      }
    } else {
      if (i->liveNeighbors == 3) {
        toAdd.insert(i);
      }
    }
    // Purge old, uninteresting neighbors.
    if (i->liveNeighbors == 0) {
      all.erase(i);
    }
  }
  toCheck.clear();
}

static long wrapAdd(long coord, int delta) {
  if (coord == std::numeric_limits<long>::max() && delta > 0) {
    return std::numeric_limits<long>::min();
  } else if (coord == std::numeric_limits<long>::min() && delta < 0) {
    return std::numeric_limits<long>::max();
  } else {
    return coord + delta;
  }
}

void Life::updateNeighborCount(std::shared_ptr<Cell> cell, int delta) {
  for (auto i : directions) {
    long x = wrapAdd(cell->x, i.first);
    long y = wrapAdd(cell->y, i.second);

    auto neighbor = std::make_shared<Cell>(x, y);
    auto existing = all.find(neighbor);
    if (existing == all.end()) {
      all.insert(neighbor);
    } else {
      neighbor = *existing;
    }

    neighbor->liveNeighbors += delta;

    toCheck.insert(neighbor);
  }
}

std::vector<Cell> Life::getAlive() const {
  // TODO(eworoshow): Copying the data is not ideal, but there is currently no good mechanism in
  // Emscripten to expose a const iterator to Javascript.
  std::vector<Cell> cells;
  cells.reserve(alive.size());
  for (auto i : alive) {
    cells.push_back(*i);
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
