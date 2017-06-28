cmake_minimum_required (VERSION 2.6)

project(Life)

set(CMAKE_CXX_FLAGS "-std=c++14 -Wall")

add_library(life_lib life.cc)

# Emscripten JS output.
add_custom_target(js)
add_custom_command(
  TARGET js POST_BUILD
  COMMAND emcc -O3 --memory-init-file 0 --bind -s NO_EXIT_RUNTIME=1 life.cc life_js_bindings.cc -o life_js_bindings.js
)

# Tests.
add_subdirectory(vendor/googletest)
enable_testing()
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
include_directories(${gmock_SOURCE_DIR}/include ${gmock_SOURCE_DIR})
add_executable(life_test life_test.cc)
target_link_libraries(life_test life_lib)
target_link_libraries(life_test gtest gmock gtest_main)
