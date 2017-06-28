#include "life.h"
#include <emscripten/bind.h>

EMSCRIPTEN_BINDINGS(Life) {
  emscripten::value_array<Cell>("Cell")
    .element(&Cell::first)
    .element(&Cell::second);

  emscripten::register_vector<Cell>("CellVector");

  emscripten::class_<Life>("Life")
    .constructor<std::vector<Cell>>()
    .function("step", &Life::step)
    .function("getAlive", &Life::getAlive);

  emscripten::function("parseCellList", &parseCellList);
}
