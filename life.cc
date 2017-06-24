#include <string>
#include <sstream>
#include <vector>
#include <iostream>

#include "life.h"

const std::vector<Cell> Life::get() const {
  std::vector<Cell> cells;
  cells.push_back((Cell) { 1, 1 });
  cells.push_back((Cell) { -2, -2 });
  cells.push_back((Cell) { std::numeric_limits<long>::min(), std::numeric_limits<long>::max() });
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
    cells.push_back((Cell) { x, y });
  }

  return cells;
}
