#ifndef LIFE_H
#define LIFE_H

#include <string>
#include <sstream>
#include <vector>
#include <iostream>

struct Cell {
  // NB: Should be int64_t but Javascript has no native support for 64-bit integers and so
  // Emscripten/Embind correspondingly has no Javascript representation for int64_t.
  // TODO(eworoshow): Templatize the numeric type.
  long x;
  long y;
};

class Life {
public:
  Life() {}

  // TODO(eworoshow): Consider real options for returning data to JS.
  const std::vector<Cell> get() const;
};

// Parses a newline-delimited set of (x, y) cells. Invalid input is skipped.
std::vector<Cell> parseCellList(std::string list);

#endif
