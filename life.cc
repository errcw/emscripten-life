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
  if (this->alive.find(cell) != this->alive.end()) {
    return;
  }
  this->all.insert(cell);
  this->toAdd.insert(cell);
  this->toCheck.insert(cell);
  this->toRemove.erase(cell);
}

void Life::step() {
  // Removals.
  for (auto i = this->toRemove.begin(); i != this->toRemove.end(); ++i) {
    this->alive.erase(*i);
    updateNeighborCount(*i, -1);
  }
  this->toRemove.clear();

  // Additions.
  for (auto i = this->toAdd.begin(); i != this->toAdd.end(); ++i) {
    this->alive.insert(*i);
    updateNeighborCount(*i, 1);
  }
  this->toAdd.clear();

  // Check possible additions or removals.
  for (auto i = this->toCheck.begin(); i != this->toCheck.end(); ++i) {
    auto c = *i;
    if (this->alive.find(c) != this->alive.end()) {
      if (c->liveNeighbors < 2 || c->liveNeighbors > 3) {
        this->toRemove.insert(c);
      }
    } else {
      if (c->liveNeighbors == 3) {
        this->toAdd.insert(c);
      }
    }
  }
  this->toCheck.clear();

  // Purge old, uninteresting neighbors.
  for (auto i = this->all.begin(); i != this->all.end(); ) {
    if ((*i)->liveNeighbors == 0) {
      i = this->all.erase(i);
    } else {
      ++i;
    }
  }
}

long wrapAdd(long coord, int delta) {
  if (coord == std::numeric_limits<long>::max() && delta > 0) {
    return std::numeric_limits<long>::min();
  } else if (coord == std::numeric_limits<long>::min() && delta < 0) {
    return std::numeric_limits<long>::max();
  } else {
    return coord + delta;
  }
}

void Life::updateNeighborCount(std::shared_ptr<Cell> cell, int delta) {
  for (auto i = this->directions.begin(); i != directions.end(); ++i) {
    long x = wrapAdd(cell->x, (*i).first);
    long y = wrapAdd(cell->y, (*i).second);

    auto neighbor = std::make_shared<Cell>(x, y);
    auto existing = this->all.find(neighbor);
    if (existing == this->all.end()) {
      this->all.insert(neighbor);
    } else {
      neighbor = *existing;
    }

    neighbor->liveNeighbors += delta;
    if (neighbor->liveNeighbors < 0) {
      neighbor->liveNeighbors = 0;
    }
    this->toCheck.insert(neighbor);
  }
}

const std::vector<Cell> Life::getAlive() const {
  // TODO(eworoshow): Copying the data is not ideal, but there is currently no good mechanism in
  // Emscripten to expose a const iterator to Javascript.
  std::vector<Cell> cells;
  cells.reserve(this->alive.size());
  for (auto i = this->alive.begin(); i != this->alive.end(); ++i) {
    cells.push_back(**i);
  }
  return cells;
}

std::vector<Cell> parseCellList(std::string list) {
  std::vector<Cell> cells;

  std::stringstream slist(list);
  std::string line;
  while (std::getline(slist, line)) {
    std::stringstream sline(line);
    char delim;
    long x, y;
    sline >> delim;
    if (delim != '(') {
      continue;
    }
    sline >> x;
    if (sline.fail()) {
      continue;
    }
    sline >> delim;
    if (delim != ',') {
      continue;
    }
    sline >> y;
    if (sline.fail()) {
      continue;
    }
    cells.push_back(Cell(x, y));
  }

  return cells;
}
