cmake_minimum_required (VERSION 2.6)

project(Life)

set(CMAKE_CXX_FLAGS "-std=c++14 -Wall")

add_library(life_lib life.cc)

# Tests.
add_subdirectory(vendor/googletest) # TODO(eworoshow): Vendor dependencies appropriately.
enable_testing()
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
include_directories(${gmock_SOURCE_DIR}/include ${gmock_SOURCE_DIR})
add_executable(life_test life_test.cc)
target_link_libraries(life_test life_lib)
target_link_libraries(life_test gtest gmock gtest_main)
