#include "life.h"
#include <emscripten/bind.h>

EMSCRIPTEN_BINDINGS(Life) {
  emscripten::value_array<Cell>("Cell")
    .element(&Cell::x)
    .element(&Cell::y);

  emscripten::register_vector<Cell>("CellVector");

  emscripten::class_<Life>("Life")
    .constructor<>()
    .function("addAt", &Life::addAt)
    .function("step", &Life::step)
    .function("getAlive", &Life::getAlive);

  emscripten::function("parseCellList", &parseCellList);
}
